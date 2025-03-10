
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== feature_provider.h ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // this file defines the class Feature Provider
    
    // Public:
      // the public class declares the constructor FeatureProvider(int feature_size, int8_t* feature_data); 
      // destructor ~FeatureProvider();
      // and the function TfLiteStatus PopulateFeatureData(int32_t last_time_in_ms, int32_t time_in_ms, int* how_many_new_slices);
    
    // Private:
      // int feature_size_      --> store the size of the the feature buffer
      // int8_t* feature_data_  --> pointer to feature data memory (holds extracted audio features)
      // bool is_first_run_;    --> tracks if it is the first time PopulateFeatureData()

  // the constructor initializes the FeatureProvider object and clears the feature data buffer by setting all values to 0

//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_FEATURE_PROVIDER_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_FEATURE_PROVIDER_H_

#include "tensorflow/lite/c/common.h"

// Binds itself to an area of memory intended to hold the input features for an
// audio-recognition neural network model, and fills that data area with the
// features representing the current audio input, for example from a microphone.
// The audio features themselves are a two-dimensional array, made up of
// horizontal slices representing the frequencies at one point in time, stacked
// on top of each other to form a spectrogram showing how those frequencies
// changed over time.

//-------------------------MANAGES-SPECTROGRAM-FEATURES-------------------------------
class FeatureProvider {
 public:
  // Create the provider, and bind it to an area of memory. This memory should
  // remain accessible for the lifetime of the provider object, since subsequent
  // calls will fill it with feature data. The provider does no memory
  // management of this data.
  FeatureProvider(int feature_size, int8_t* feature_data);
  ~FeatureProvider();

  // Fills the feature data with information from audio inputs, and returns how
  // many feature slices were updated.
  TfLiteStatus PopulateFeatureData(int32_t last_time_in_ms, int32_t time_in_ms,
                                   int* how_many_new_slices);

 private:
  int feature_size_;
  int8_t* feature_data_;
  // Make sure we don't try to use cached information if this is the first call
  // into the provider.
  bool is_first_run_;
};

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_FEATURE_PROVIDER_H_
