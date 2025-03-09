
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== featur_provider.cpp ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // this file includes the  
    // it uses other functions such as: 
      // InitialiseMicroFeatures()  -> 
      // GenerateMicroFeatures()    -> micro_features_micro_features_generator.cpp

    // Constr
//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#include "feature_provider.h"
#include "audio_provider.h" // for audio processing
#include "micro_features_micro_features_generator.h"
#include "micro_features_micro_model_settings.h"
#include "tensorflow/lite/micro/micro_log.h"

// ------------------------CONSTRUCTOR-------------------------------

FeatureProvider::FeatureProvider(int feature_size, int8_t* feature_data)
    : feature_size_(feature_size),
      feature_data_(feature_data),
      is_first_run_(true) {
  // Initialize the feature data to default values.
  for (int n = 0; n < feature_size_; ++n) {
    feature_data_[n] = 0;
  }
}

// ------------------------DESTRUCTOR-------------------------------

FeatureProvider::~FeatureProvider() {}

//---------------------------------------------------------------------PopulateFeatureData----------------------------------------------------------------------------------------------

// feature_provider.cpp --> Fills the feature data with information from audio inputs, and returns how many feature slices were updated.
TfLiteStatus FeatureProvider::PopulateFeatureData(int32_t last_time_in_ms,
                                                  int32_t time_in_ms,
                                                  int* how_many_new_slices) {
  
  // ---------------------FEATURE-SIZE-CHECK-------------------------------
  // check if the feature size is correct
  if (feature_size_ != kFeatureElementCount) { // if there is an error, it logs it and returns an error code. 
    MicroPrintf("Requested feature_data_ size %d doesn't match %d",
                feature_size_, kFeatureElementCount);
    return kTfLiteError;
  }

  // ---------------------COMPUTE-SLICES-------------------------------
  // finds how many new slices of 20ms samples we need
  // Quantize the time into steps as long as each window stride, so we can figure out which audio data we need to fetch.
  const int last_step = (last_time_in_ms / kFeatureSliceStrideMs);
  // Number of new 20ms slices from which we can take 30ms samples
  int slices_needed =
      ((((time_in_ms - last_time_in_ms) - kFeatureSliceDurationMs) *
        kFeatureSliceStrideMs) /
           kFeatureSliceStrideMs +
       kFeatureSliceStrideMs) /
      kFeatureSliceStrideMs;

  // ---------------------FIRST-RUN-INITIALIZATION-------------------------------
  // If this is the first call, make sure we don't use any cached information. //########################################################################################### what are cached info?
  if (is_first_run_) { 
    TfLiteStatus init_status = InitializeMicroFeatures(); // defined in (micro_features_micro_features_generator.h/.cpp)
                                                          // this function sets up extraction for audio data.
                                                          // and prepares the system to convert raw sound into numerical features
    if (init_status != kTfLiteOk) {
      return init_status;
    }
    is_first_run_ = false;
    return kTfLiteOk;
  }

  // ---------------------HANDLE-LARGE-SLICES-------------------------------
  
  if (slices_needed > kFeatureSliceCount) { //###########################################################################################
    slices_needed = kFeatureSliceCount;//###########################################################################################
  }
  if (slices_needed == 0) { //                           why? 
    return kTfLiteOk;
  }
  *how_many_new_slices = slices_needed;

  // ---------------------SHIFT-OLD-DATA-UPWARDS-------------------------------
  // keep some old slices and drop oldest ones
  const int slices_to_keep = kFeatureSliceCount - slices_needed;
  const int slices_to_drop = kFeatureSliceCount - slices_to_keep;

  // If we can avoid recalculating some slices, just move the existing data
  // up in the spectrogram, to perform something like this:
  // last time = 80ms          current time = 120ms
  // +-----------+             +-----------+
  // | data@20ms |         --> | data@60ms |
  // +-----------+       --    +-----------+
  // | data@40ms |     --  --> | data@80ms |
  // +-----------+   --  --    +-----------+
  // | data@60ms | --  --      |  <empty>  |
  // +-----------+   --        +-----------+
  // | data@80ms | --          |  <empty>  |
  // +-----------+             +-----------+

  // move existing feature slices up to make more space for new ones 
  if (slices_to_keep > 0) {
    for (int dest_slice = 0; dest_slice < slices_to_keep; ++dest_slice) {
      int8_t* dest_slice_data =
          feature_data_ + (dest_slice * kFeatureSliceSize);
      const int src_slice = dest_slice + slices_to_drop;
      const int8_t* src_slice_data =
          feature_data_ + (src_slice * kFeatureSliceSize);
      for (int i = 0; i < kFeatureSliceSize; ++i) {
        dest_slice_data[i] = src_slice_data[i];
      }
    }
  }

  // ---------------------EXTRACT-NEW-SAMPLES-------------------------------
  // Any slices that need to be filled in with feature data have their
  // appropriate audio data pulled, and features calculated for that slice.
  if (slices_needed > 0) {
    for (int new_slice = slices_to_keep; new_slice < kFeatureSliceCount;
         ++new_slice) {
      const int new_step = last_step + (new_slice - slices_to_keep);
      const int32_t slice_start_ms = (new_step * kFeatureSliceStrideMs);
      int16_t* audio_samples = nullptr;
      int audio_samples_size = 0;
      GetAudioSamples(slice_start_ms, kFeatureSliceDurationMs,
                      &audio_samples_size, &audio_samples);

      // ensures that the audio buffer is the correct size.          
      constexpr int wanted =
          kFeatureSliceDurationMs * (kAudioSampleFrequency / 1000);
      if (audio_samples_size != wanted) {
        MicroPrintf("Audio data size %d too small, want %d", audio_samples_size,
                    wanted);
        return kTfLiteError;
      }

      // ---------------------CONVERT-AUDIO-TO-FEATURES-------------------------------
      int8_t* new_slice_data = feature_data_ + (new_slice * kFeatureSliceSize);
      size_t num_samples_read;
      TfLiteStatus generate_status = GenerateMicroFeatures( // GenerateMicroFeatures() is defined within micro_features_micro_features_generator.cpp
                                                            // it processes raw audio and converts it into feature data. 
          audio_samples, audio_samples_size, kFeatureSliceSize, new_slice_data,
          &num_samples_read);
      if (generate_status != kTfLiteOk) {
        return generate_status;
      }
    }
  }
  return kTfLiteOk;
}
