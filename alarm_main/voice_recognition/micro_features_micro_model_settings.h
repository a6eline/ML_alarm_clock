
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== micro_features_micro_model_settings.h ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // defining important settings for the voice recgnition model to run on the Arduino.
    // it helps the AI model by defining:
      // audio settings such as sample size, frequency etc.
      // how the input is processed
      // the output categories (words to be recognized)
    
//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MICRO_FEATURES_MICRO_MODEL_SETTINGS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MICRO_FEATURES_MICRO_MODEL_SETTINGS_H_

//---------------------------------------------------------------------CONSTANT-EXPRESSIONS ----------------------------------------------------------------------------------------------

// Keeping these as constant expressions allow us to allocate fixed-sized array on the stack for our working memory.

// ------------------------AUDIO-SETTINGS-------------------------------

// The size of the input time series data we pass to the FFT to produce the
// frequency information. This has to be a power of two, and since we're dealing
// with 30ms of 16KHz inputs, which means 480 samples, this is the next value.
constexpr int kMaxAudioSampleSize = 512;      // kMaxAudioSampleSize   -> the number of audio samples the AI can process at a time
constexpr int kAudioSampleFrequency = 16000;  // kAudioSampleFrequency -> the number of samples per second

// ------------------------FEATURE-EXTRACTION-SETTINGS-------------------------------

// The following values are derived from values used during model training.
// If you change the way you preprocess the input, update all these constants.
constexpr int kFeatureSliceSize = 40;       // kFeatureSliceSize    -> each slice contains 40 numbers (features)
constexpr int kFeatureSliceCount = 49;      // kFeatureSliceCount   -> the AI looks at 49 slices at a time to recognize speech
constexpr int kFeatureElementCount =        // kFeatureElementCount -> the total number of features
  (kFeatureSliceSize * kFeatureSliceCount);
constexpr int kFeatureSliceStrideMs = 20;   // kFeatureSliceStrideMs    -> the slices overlap by 20 ms to capture smooth transitions
constexpr int kFeatureSliceDurationMs = 30; // kFeatureSliceDurationMs  -> each slice represents 30 ms of sound

// ------------------------WORD-LABELS-SETTINGS-------------------------------

// Variables for the model's output categories.
constexpr int kSilenceIndex = 0;  // represents no sound
constexpr int kUnknownIndex = 1;  // represents the words AI doesnt understand

// If you modify the output categories, you need to update the following values.
constexpr int kCategoryCount = 4; // there are 4 categories the AI can recognize
extern const char* kCategoryLabels[kCategoryCount]; // full array is in micro_features_micro_model_settings.cpp

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MICRO_FEATURES_MICRO_MODEL_SETTINGS_H_
