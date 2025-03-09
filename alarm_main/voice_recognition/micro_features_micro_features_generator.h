
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== micro_features_micro_features_generator.h ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // functions in here are used in "FeatureProvider::PopulateFeatureData()"" 
        // ^ this is in "feature_provider.cpp"
    // this file configures the microphone filters 
    // and also extracts them by converting it into a format AI can understand 
    
//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MICRO_FEATURES_MICRO_FEATURES_GENERATOR_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MICRO_FEATURES_MICRO_FEATURES_GENERATOR_H_

#include "tensorflow/lite/c/common.h"

//---------------------------------------------------------------------InitializeMicroFeatures----------------------------------------------------------------------------------------------

// Sets up any resources needed for the feature generation pipeline.
TfLiteStatus InitializeMicroFeatures();


//---------------------------------------------------------------------GenerateMicroFeatures----------------------------------------------------------------------------------------------

// Converts audio sample data into a more compact form that's appropriate for
// feeding into a neural network.
TfLiteStatus GenerateMicroFeatures(const int16_t* input, int input_size,
                                   int output_size, int8_t* output,
                                   size_t* num_samples_read);

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MICRO_FEATURES_MICRO_FEATURES_GENERATOR_H_
