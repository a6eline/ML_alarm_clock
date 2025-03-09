
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== recognize_commands.cpp ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // as the file name suggests, this is to recognize voice commands
    // it takes results from a machine learning model
    // this listens to audio and detects spoken words. 
    // then it will smoothen out noisy results and figure out when real coommand has been spoken

    // the functions are used within micro_speech.ino 
    
//---------------------------------------------------------------------PREPROCESSOR----------------------------------------------------------------------------------------------

#include "recognize_commands.h"

#include <limits>

#undef DEBUG_MICRO_SPEECH // define if you want debug, undef if not wanted

//---------------------------------------------------------------------RecognizeCommands----------------------------------------------------------------------------------------------

// CONSTRUCTOR: recognise_commands.cpp --> this stores the user-defined settings, initialises previous (so we can compare) and starts with silence. 
RecognizeCommands::RecognizeCommands(int32_t average_window_duration_ms,  // average_window_duration_ms -> How long (ms) to average results to smooth out noise.
                                     uint8_t detection_threshold,         // detection_threshold        -> How confident the model needs to be before accepting a command.
                                     int32_t suppression_ms,              // suppression_ms             -> Time after a detection before allowing another command.
                                     int32_t minimum_count)               // minimum_count              -> Minimum results needed before making a decision.
    : average_window_duration_ms_(average_window_duration_ms),
      detection_threshold_(detection_threshold),
      suppression_ms_(suppression_ms),
      minimum_count_(minimum_count),
      previous_results_() {
  previous_top_label_ = kCategoryLabels[0];  // starts with silence as the previous recognized command
  previous_top_label_time_ = std::numeric_limits<int32_t>::min();
}

//---------------------------------------------------------------------ProcessLatestResults----------------------------------------------------------------------------------------------

// recognise_commands.cpp --> decides if a new command was spoken
TfLiteStatus RecognizeCommands::ProcessLatestResults(const TfLiteTensor* latest_results,  // latest_results   -> the ML model's predictions for current audio input
                                                     const int32_t current_time_ms,       // current_time_ms  -> the timestamp of this result
                                                     const char** found_command,          // found_command    -> detected command (yes or no etc)
                                                     uint8_t* score,                      // score            -> confidence score for detected command.
                                                     bool* is_new_command) {              // is_new_command   -> true if a new command was just spoken.

  // ------------------------FORMAT-CHECK-------------------------------
  if ((latest_results->dims->size != 2) ||
      (latest_results->dims->data[0] != 1) ||
      (latest_results->dims->data[1] != kCategoryCount)) { // if it is wrong format
    MicroPrintf(
        "The results for recognition should contain %d elements, but there are "
        "%d in an %d-dimensional shape",
        kCategoryCount, latest_results->dims->data[1],
        latest_results->dims->size);
    return kTfLiteError;
  }

  // ------------------------FORMAT-CHECK-------------------------------
  if (latest_results->type != kTfLiteInt8) { 
    MicroPrintf(
        "The results for recognition should be int8_t elements, but are %d",
        latest_results->type);
    return kTfLiteError;
  }

  // ------------------------CORRECT-ORDER-------------------------------
  // ensure the results are fed in correct order
  if ((!previous_results_.empty()) &&
      (current_time_ms < previous_results_.front().time_)) {
    // if order is wrong, then 
    MicroPrintf(
        "Results must be fed in increasing time order, but received a "
        "timestamp of %d that was earlier than the previous one of %d",
        current_time_ms, previous_results_.front().time_);
    return kTfLiteError;
  }

  // ------------------------REMOVE-OLD-RESULTS-------------------------------
  // Prune any earlier results that are too old for the averaging window.
  const int64_t time_limit = current_time_ms - average_window_duration_ms_;
  while ((!previous_results_.empty()) &&
         previous_results_.front().time_ < time_limit) {
    previous_results_.pop_front();
  }

  // ------------------------ADD-NEW-RESULTS-------------------------------

  // Add the latest results to the head of the queue.
  previous_results_.push_back({current_time_ms, latest_results->data.int8});

  // ------------------------IF-FEW-RESULTS-------------------------------

  // If there are too few results, assume the result will be unreliable and bail.
  // it will return to the last known command
  const int64_t how_many_results = previous_results_.size();
  const int64_t earliest_time = previous_results_.front().time_;
  const int64_t samples_duration = current_time_ms - earliest_time;
  if ((how_many_results < minimum_count_) ||
      (samples_duration < (average_window_duration_ms_ / 4))) {
    *found_command = previous_top_label_;
    *score = 0;
    *is_new_command = false;
    return kTfLiteOk;
  }

  // ------------------------CALCULATE-AVERAGE------------------------------

  // Calculate the average score across all the results in the window.
  int32_t average_scores[kCategoryCount];
  for (int offset = 0; offset < previous_results_.size(); ++offset) {
    PreviousResultsQueue::Result previous_result =
        previous_results_.from_front(offset);
    const int8_t* scores = previous_result.scores;
    for (int i = 0; i < kCategoryCount; ++i) {
      if (offset == 0) {
        average_scores[i] = scores[i] + 128;
      } else {
        average_scores[i] += scores[i] + 128;
      }
    }
  }
  for (int i = 0; i < kCategoryCount; ++i) {
    average_scores[i] /= how_many_results;
  }

  // ------------------------CALCULATE-HIGHEST------------------------------

  // Find the current highest scoring category.
  int current_top_index = 0;
  int32_t current_top_score = 0;
  for (int i = 0; i < kCategoryCount; ++i) {
    if (average_scores[i] > current_top_score) {
      current_top_score = average_scores[i];
      current_top_index = i;
    }
  }
  const char* current_top_label = kCategoryLabels[current_top_index];


  // ------------------------CONTROL-TIMING------------------------------

  // If we've recently had another label trigger, assume one that occurs too soon afterwards is a bad result.
  int64_t time_since_last_top;
  if ((previous_top_label_ == kCategoryLabels[0]) ||
      (previous_top_label_time_ == std::numeric_limits<int32_t>::min())) {
    time_since_last_top = std::numeric_limits<int32_t>::max();
  } else {
    time_since_last_top = current_time_ms - previous_top_label_time_;
  }
  if ((current_top_score > detection_threshold_) &&
      ((current_top_label != previous_top_label_) ||
       (time_since_last_top > suppression_ms_))) {

// ------------------------DEBUG------------------------------

#ifdef DEBUG_MICRO_SPEECH
    MicroPrintf("Scores: s %d u %d y %d n %d  %s -> %s", average_scores[0],
                average_scores[1], average_scores[2], average_scores[3],
                previous_top_label_, current_top_label);
#endif  // DEBUG_MICRO_SPEECH
    previous_top_label_ = current_top_label;
    previous_top_label_time_ = current_time_ms;
    *is_new_command = true;
  } else {
#ifdef DEBUG_MICRO_SPEECH
    if (current_top_label != previous_top_label_) {
      MicroPrintf("#Scores: s %d u %d y %d n %d  %s -> %s", average_scores[0],
                  average_scores[1], average_scores[2], average_scores[3],
                  previous_top_label_, current_top_label);
      previous_top_label_ = current_top_label;
    }
#endif  // DEBUG_MICRO_SPEECH


    *is_new_command = false;
  }
  *found_command = current_top_label;
  *score = current_top_score;

  return kTfLiteOk;
}
