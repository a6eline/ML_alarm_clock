
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

// MODIFICATIONS: NOT NEEDED

//==================================================================== arduino_audio_provider.cpp ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // handles audio input using PDM microphone 
    // initializes the microphone
    // captures audio samples
    // processes them
    // makes them available for the TensorFlow Lite inferene
    // also supports serial communication

//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE) // if the board is ARDUINO, but NOT the NANO33BLE
#define ARDUINO_EXCLUDE_CODE // then define this
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE // executes below IF this macro is not defined. 

#include <algorithm>  // standard C++ library used for algorithm sorting 
#include <cmath>      // for mathematical functions like square root/trig
#include "PDM.h"      // for the microphone interface

#include "audio_provider.h"                       // its header file
#include "micro_features_micro_model_settings.h"  // contains settings for model features
#include "tensorflow/lite/micro/micro_log.h"      // helps log info for debugging
#include "test_over_serial/test_over_serial.h"    // handles testing functionality through serial

using namespace test_over_serial;

//---------------------------------------------------------------------GLOBALS ----------------------------------------------------------------------------------------------

namespace {
bool g_is_audio_initialized = false; // flag to track if the microphone has been initalized

//---------------------------BUFFER-----------------------------------
// An internal buffer able to fit 16x our sample size
constexpr int kAudioCaptureBufferSize = DEFAULT_PDM_BUFFER_SIZE * 16; // kAudioCaptureBufferSize  -> This constant defines the size of the audio buffer. 
                                                                      // It’s set to be 16 times the DEFAULT_PDM_BUFFER_SIZE, 
                                                                      // meaning the buffer can hold multiple audio samples to handle continuous capture.
//-------------------------AUDIO-FILES---------------------------------
int16_t g_audio_capture_buffer[kAudioCaptureBufferSize];        // g_audio_capture_buffer   -> for raw audio files
int16_t g_audio_output_buffer[kMaxAudioSampleSize];             // g_audio_output_buffer    -> for processed audio samples to be used by the model

//------------------------AUDIO-TIMESTAMP------------------------------
// Mark as volatile so we can check in a while loop to see if any samples have arrived yet.
volatile int32_t g_latest_audio_timestamp = 0;                  // g_test_audio_timestamp -> timestamp to hold the time of the latest audio sample in mills

//--------------------------TEST-SAMPLE---------------------------------
// test_over_serial sample index
uint32_t g_test_sample_index;                                   // g_test_sample_index -> used to handle test samples during serial comm testing
// test_over_serial silence insertion flag
bool g_test_insert_silence = true;                              // g_test_insert_silence -> flag to test mode to determine whether silence should be inserted 
}  // namespace


//-------------------------------------------------------------------CaptureSamples------------------------------------------------------------------------------------------------

// arduino_audio_provider.cpp --> reads audio sample files using PDM, processes them and stores it within a buffer. also updates the audio timestamp
void CaptureSamples() {
  const int number_of_samples = DEFAULT_PDM_BUFFER_SIZE / 2; // This is how many bytes of new data we have each time this is called 

  const int32_t time_in_ms = // Calculate what timestamp the last audio sample represents
      g_latest_audio_timestamp +
      (number_of_samples / (kAudioSampleFrequency / 1000));

  const int32_t start_sample_offset = // Determine the index, in the history of all samples, of the last sample
      g_latest_audio_timestamp * (kAudioSampleFrequency / 1000);

  const int capture_index = start_sample_offset % kAudioCaptureBufferSize; // Determine the index of this sample in our ring buffer
  
  // Read the data to the correct place in our buffer
  int num_read = PDM.read(g_audio_capture_buffer + capture_index, DEFAULT_PDM_BUFFER_SIZE);

  if (num_read != DEFAULT_PDM_BUFFER_SIZE) {
    MicroPrintf("### short read (%d/%d) @%dms", num_read, DEFAULT_PDM_BUFFER_SIZE, time_in_ms);
    while (true) {
      // NORETURN
    }
  }
  // This is how we let the outside world know that new audio data has arrived.
  g_latest_audio_timestamp = time_in_ms;
}

//-------------------------------------------------------------------InitAudioRecording------------------------------------------------------------------------------------------------

// arduino_audio_provider.cpp --> initializes the PDM mic to start recieving audio data. 
TfLiteStatus InitAudioRecording() {
  if (!g_is_audio_initialized) {
    // Hook up the callback that will be called with each sample
    PDM.onReceive(CaptureSamples);
    // Start listening for audio: MONO @ 16KHz
    PDM.begin(1, kAudioSampleFrequency);
    // gain: -20db (min) + 6.5db (13) + 3.2db (builtin) = -10.3db
    PDM.setGain(13);
    // Block until we have our first audio sample
    while (!g_latest_audio_timestamp) {
    }
    g_is_audio_initialized = true;
  }

  return kTfLiteOk;
}


//-------------------------------------------------------------------GetAudioSamples------------------------------------------------------------------------------------------------

// arduino_audio_provider.cpp --> retrieves audio samples from the capture buffer for a specific time window
TfLiteStatus GetAudioSamples(int start_ms, int duration_ms,
                             int* audio_samples_size, int16_t** audio_samples) {
  // This next part should only be called when the main thread notices that the
  // latest audio sample data timestamp has changed, so that there's new data
  // in the capture ring buffer. The ring buffer will eventually wrap around and
  // overwrite the data, but the assumption is that the main thread is checking
  // often enough and the buffer is large enough that this call will be made
  // before that happens.

  // Determine the index, in the history of all samples, of the first sample we want
  const int start_offset = start_ms * (kAudioSampleFrequency / 1000);
  // Determine how many samples we want in total
  const int duration_sample_count =
      duration_ms * (kAudioSampleFrequency / 1000);
  for (int i = 0; i < duration_sample_count; ++i) {
    // For each sammple, transform its index in the history of all samples into
    // its index in g_audio_capture_buffer
    const int capture_index = (start_offset + i) % kAudioCaptureBufferSize;
    // Write the sample to the output buffer
    g_audio_output_buffer[i] = g_audio_capture_buffer[capture_index];
  }

  // Set pointers to provide access to the audio
  *audio_samples_size = duration_sample_count;
  *audio_samples = g_audio_output_buffer;

  return kTfLiteOk;
}


namespace {
//-------------------------------------------------------------------InsertSilence------------------------------------------------------------------------------------------------

// arduino_audio_provider.cpp --> insert zero values into the buffer for test mode. 
void InsertSilence(const size_t len, int16_t value) {
  for (size_t i = 0; i < len; i++) {
    const size_t index = (g_test_sample_index + i) % kAudioCaptureBufferSize;
    g_audio_capture_buffer[index] = value;
  }
  g_test_sample_index += len;
}

//-------------------------------------------------------------------ProcessTestInput------------------------------------------------------------------------------------------------

// arduino_audio_provider.cpp --> handles test mode input through serial communication
int32_t ProcessTestInput(TestOverSerial& test) {
  constexpr size_t samples_16ms = ((kAudioSampleFrequency / 1000) * 16);

  InputHandler handler = [](const InputBuffer* const input) {
    if (0 == input->offset) {
      // don't insert silence
      g_test_insert_silence = false;
    }

    for (size_t i = 0; i < input->length; i++) {
      const size_t index = (g_test_sample_index + i) % kAudioCaptureBufferSize;
      g_audio_capture_buffer[index] = input->data.int16[i];
    }
    g_test_sample_index += input->length;

    if (input->total == (input->offset + input->length)) {
      // allow silence insertion again
      g_test_insert_silence = true;
    }
    return true;
  };

  test.ProcessInput(&handler);

  if (g_test_insert_silence) {
    // add 16ms of silence just like the PDM interface
    InsertSilence(samples_16ms, 0);
  }

  // Round the timestamp to a multiple of 64ms,
  // This emulates the PDM interface during inference processing.
  g_latest_audio_timestamp = (g_test_sample_index / (samples_16ms * 4)) * 64;
  return g_latest_audio_timestamp;
}

}  // namespace

//-------------------------------------------------------------------LatestAudioTimestamp------------------------------------------------------------------------------------------------

// arduino_audio_provider.cpp --> returns latest timestamp, handling both normal and test mode.
int32_t LatestAudioTimestamp() {
  TestOverSerial& test = TestOverSerial::Instance(kAUDIO_PCM_16KHZ_MONO_S16);
  if (!test.IsTestMode()) {
    // check serial port for test mode command
    test.ProcessInput(nullptr);
  }
  if (test.IsTestMode()) {
    if (g_is_audio_initialized) {
      // stop capture from hardware
      PDM.end();
      g_is_audio_initialized = false;
      g_test_sample_index =
          g_latest_audio_timestamp * (kAudioSampleFrequency / 1000);
    }
    return ProcessTestInput(test);
  } else {
    // CaptureSamples() updated the timestamp
    return g_latest_audio_timestamp;
  }
  // NOTREACHED
}

#endif  // ARDUINO_EXCLUDE_CODE
