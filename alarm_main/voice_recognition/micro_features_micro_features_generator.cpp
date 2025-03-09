
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== micro_features_micro_features_generator.cpp====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // functions in here are used in "FeatureProvider::PopulateFeatureData()"" 
        // ^ this is in "feature_provider.cpp"
    // this file configures the microphone filters 
    // and also extracts them by converting it into a format AI can understand 

    // InitializeMicroFeatures()
      // configures key mic details such as:
        // window size
        // filter bank
        // noise reduction
        // PCAN gain control
        // log scaling
        // load config
    
    // GenerateMicroFeatures()
        // converts raw audio into AI friendly features
        // if you see objects called frontend, it is probably from: 
          // "tensorflow/lite/experimental/microfrontend/lib/frontend.h"

//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#include "micro_features_micro_features_generator.h"

#include <cmath>
#include <cstring>

#include "micro_features_micro_model_settings.h"
#include "tensorflow/lite/experimental/microfrontend/lib/frontend.h"
#include "tensorflow/lite/experimental/microfrontend/lib/frontend_util.h"
#include "tensorflow/lite/micro/micro_log.h"

// Configure FFT to output 16 bit fixed point.
#define FIXED_POINT 16

// ------------------------NAMESPACE-------------------------------
namespace {
FrontendState g_micro_features_state; // stores the settings/configurations of the microphone's pre-processing
                                      // struct FrontendState is from the file: 
bool g_is_first_time = true;  // bool to avoid setting up multiple times

}  // namespace

//---------------------------------------------------------------------InitializeMicroFeatures----------------------------------------------------------------------------------------------

// micro_features_micro_features_generator.cpp --> configuring microphone filters and effects before recording 
TfLiteStatus InitializeMicroFeatures() {
  FrontendConfig config;

  // ------------------------WINDOW-SIZE-------------------------------
  config.window.size_ms = kFeatureSliceDurationMs;    // window size: how long we anaylyze audio chunks 
  config.window.step_size_ms = kFeatureSliceStrideMs; // step size:   how much we slide forward each time we analyze new chunks
  
  // ------------------------FILTER-BANK-------------------------------
  config.filterbank.num_channels = kFeatureSliceSize; // how many frequency banss we device the audio into
                                                      // for example, humans hear low, middle and high sounds. 
                                                      // this does the same for sounds for analysis
  config.filterbank.lower_band_limit = 125.0;   // ignores sounds lower than 125 Hz
  config.filterbank.upper_band_limit = 7500.0;  // ignores sounds higher than 7500 Hz
  
  // ------------------------NOISE-REDUCTION-------------------------------
  config.noise_reduction.smoothing_bits = 10;         // the higher this value, the more aggresive the noise reduction
  config.noise_reduction.smoothing_bits = 10;           // not sure why it was declared twice, probably a mistake
  config.noise_reduction.even_smoothing = 0.025;      // controls smoothing for even-numbered frequency bands
  config.noise_reduction.odd_smoothing = 0.06;        // controls smoothing for odd-numbered frequency bands
  config.noise_reduction.min_signal_remaining = 0.05; // prevents over-smoothing by keeping at least 5% of original signal
  
  // ------------------------PCAN-GAIN-CONTROL-------------------------------
  config.pcan_gain_control.enable_pcan = 1; // automatic volume control - if someone shouts it will reduce the volume
  config.pcan_gain_control.strength = 0.95; // how aggresively volume is adjusted
  config.pcan_gain_control.offset = 80.0;   // how much quieter sonds should be 
  config.pcan_gain_control.gain_bits = 21;  // prevision of gain control calculations

  // ------------------------LOG-SCALING-------------------------------
  config.log_scale.enable_log = 1;  // turns on logarithmic scaling
  config.log_scale.scale_shift = 6; // adjusts how strong the log scaling is

  // ------------------------LOAD-CONFIG-------------------------------

  // this function loads the cofig settings aboveinto the address of g_micro_features_state
  // it is like saving settings
  if (!FrontendPopulateState(&config, &g_micro_features_state,
                             kAudioSampleFrequency)) {
    MicroPrintf("FrontendPopulateState() failed");
    return kTfLiteError;
  }
  g_is_first_time = true;
  return kTfLiteOk;
}

//---------------------------------------------------------------------GenerateMicroFeatures----------------------------------------------------------------------------------------------

// micro_features_micro_features_generator.cpp --> extracts the audio feautures
// like taking the recorded sound and breaking it into numbers AI can understand
TfLiteStatus GenerateMicroFeatures(const int16_t* input, // audio samples
                                   int input_size,
                                   int output_size, 
                                   int8_t* output,
                                   size_t* num_samples_read) {
  // makes a pointer variable for the input  
  const int16_t* frontend_input;
  if (g_is_first_time) {  // checks if it is the first time or not. 
    frontend_input = input;
    g_is_first_time = false;
  } else {
    frontend_input = input;
  } 
  // why is "frontend_input = input;"" written twice? probably an accident.

  // ------------------------PROCESS-INPUT-------------------------------

  // process the input using g_micro_feature_state settings and converts it to frontend_output
  FrontendOutput frontend_output = FrontendProcessSamples( //  this 
      &g_micro_features_state, frontend_input, input_size, num_samples_read);

  // ------------------------LOAD-CONFIG-------------------------------

  for (size_t i = 0; i < frontend_output.size; ++i) {
    // These scaling values are derived from those used in input_data.py in the
    // training pipeline.
    // The feature pipeline outputs 16-bit signed integers in roughly a 0 to 670
    // range. In training, these are then arbitrarily divided by 25.6 to get
    // float values in the rough range of 0.0 to 26.0. This scaling is performed
    // for historical reasons, to match up with the output of other feature
    // generators.
    // The process is then further complicated when we quantize the model. This
    // means we have to scale the 0.0 to 26.0 real values to the -128 to 127
    // signed integer numbers.
    // All this means that to get matching values from our integer feature
    // output into the tensor input, we have to perform:
    // input = (((feature / 25.6) / 26.0) * 256) - 128
    // To simplify this and perform it in 32-bit integer math, we rearrange to:
    // input = (feature * 256) / (25.6 * 26.0) - 128

    // ------------------------SCALE-DATA-------------------------------
    
    // scaling the sound data into small range
    constexpr int32_t value_scale = 256;
    constexpr int32_t value_div = static_cast<int32_t>((25.6f * 26.0f) + 0.5f);
    int32_t value =
        ((frontend_output.values[i] * value_scale) + (value_div / 2)) /
        value_div;
    value -= 128;
    if (value < -128) {
      value = -128;
    } if (value > 127) {
      value = 127;
    }
    output[i] = value;
  }

  return kTfLiteOk;
}

//---------------------------------------------------------------------TESTING----------------------------------------------------------------------------------------------

// This is not exposed in any header, and is only used for testing, to ensure
// that the state is correctly set up before generating results.
void SetMicroFeaturesNoiseEstimates(const uint32_t* estimate_presets) {
  for (int i = 0; i < g_micro_features_state.filterbank.num_channels; ++i) {
    g_micro_features_state.noise_reduction.estimate[i] = estimate_presets[i];
  }
}
