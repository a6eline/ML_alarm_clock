
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== recognize_commands.h ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // this header file defines the structure of how command recognition works
    // it contains:
      // PreviousResultsQueue 
        // this stores the past results from the ML model and timestamps
        // this is sort of like a list that stores past speech recognition results
        // it allows adding and removing results in an orderly way
        // and it helps system look back at past results to make better predictions

      // RecognizeCommands
        // this smooths out the predictions over time
      
      // the functions are used within micro_speech.ino 
    
//---------------------------------------------------------------------MODEL----------------------------------------------------------------------------------------------

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_RECOGNIZE_COMMANDS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_RECOGNIZE_COMMANDS_H_

#include <cstdint>

#include "micro_features_micro_model_settings.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_log.h"

//-----------------------------------------------------------------CLASS-PreviousResultsQueue----------------------------------------------------------------------------------------------

// Partial implementation of std::dequeue, just providing the functionality
// that's needed to keep a record of previous neural network results over a
// short time period, so they can be averaged together to produce a more
// accurate overall prediction. This doesn't use any dynamic memory allocation
// so it's a better fit for microcontroller applications, but this does mean
// there are hard limits on the number of results it can store.

// recognize_commands.h --> CLASS to store past results from ML model
  // this class acts like a circular queue (FIFO buffer) to store past ML results
class PreviousResultsQueue {
 public:
  // initialise front_index_ and size_ to 0
  PreviousResultsQueue() : front_index_(0), size_(0) {}

  // ------------------------INFERENCE-RESULTS-------------------------------
  
  // STRUCT: PreviousResultsQueue --> recognize_commands.h -->  Data structure that holds an inference result, and the time when it was recorded.
  struct Result { 
    Result() : time_(0), scores() {} 
    Result(int32_t time, int8_t* input_scores) : time_(time) { // parametized constructor 
      for (int i = 0; i < kCategoryCount; ++i) {
        scores[i] = input_scores[i];
      }
    }
    int32_t time_; // the time when result was recorded
    int8_t scores[kCategoryCount]; // the ML model's confidence score
  };

  // ------------------------QUEUE-FUNCTIONS-------------------------------
  // recognize_commands.h -->  for queue functions, returns number of stored results  
  int size() { return size_; } 

  // recognize_commands.h -->  for queue functions, returns true of the queue is rempty.
  bool empty() { return size_ == 0; } 

  // recognize_commands.h --> for queue functions, returns oldest result (front of queue)
  Result& front() { return results_[front_index_]; } 

  // recognize_commands.h --> for queue functions, adds a new result 
  Result& back() { 
    int back_index = front_index_ + (size_ - 1); // finding yhe most recent entry (last added item)
    if (back_index >= kMaxResults) { // if the back_index exceeds kMaxResults
      back_index -= kMaxResults; // it wraps around
    }
    return results_[back_index];
  }

    if (size() >= kMaxResults) {
      MicroPrintf("Couldn't push_back latest result, too many already!");
      return;
    }
    size_ += 1;
    back() = entry;
  }

  // ------------------------REMOVE-RESULTS-------------------------------

  // FUNCTION: PreviousResultsQueue --> removes the oldest result
  Result pop_front() {
    if (size() <= 0) { // if the queue is empty
      MicroPrintf("Couldn't pop_front result, none present!");
      return Result();
    }
    Result result = front();  // get the earliest stored result with front()
    front_index_ += 1;        // moves front_index_ forward (removing the result)
    if (front_index_ >= kMaxResults) { // if front_index goes out of bounds, reset it to 0
      front_index_ = 0;       // circular buffer, reset it back to 0
    }
    size_ -= 1; // reduce the count of stored results, now the queue has one less result
    return result;
  }

  // ------------------------REMOVE-RESULTS-------------------------------

  // Most of the functions are duplicates of dequeue containers, but this
  // is a helper that makes it easy to iterate through the contents of the
  // queue.
  
  // FUNCTION: PreviousResultsQueue --> retreives a specific result based on its position
  Result& from_front(int offset) {
    if ((offset < 0) || (offset >= size_)) { // checks if the requested offset is invalid (either neg or too large)
      MicroPrintf("Attempt to read beyond the end of the queue!");
      offset = size_ - 1; // if invalid, it will corrext it to the point of the last stored result
    }
    int index = front_index_ + offset; // index -> where the requested result lvies inside the array
    if (index >= kMaxResults) { // if index goes beyond queues max, it wraps using... 
      index -= kMaxResults; // ... this
    }
    return results_[index]; // returning the correct result from queue
  }

  // ------------------------PRIV-VARIABLES-------------------------------
 private:
  static constexpr int kMaxResults = 50; // maximum result scores which can be held in ...
  Result results_[kMaxResults];          // ...results_[], the array of all scores.

  int front_index_; // front_index_ -> index position to the oldest result in the queue
  int size_;        // size_        -> the number of results currently in the queue
};

//---------------------------------------------------------------------CLASS-RecognizeCommands----------------------------------------------------------------------------------------------

// This class is designed to apply a very primitive decoding model on top of the
// instantaneous results from running an audio recognition model on a single
// window of samples. It applies smoothing over time so that noisy individual
// label scores are averaged, increasing the confidence that apparent matches
// are real.
// To use it, you should create a class object with the configuration you
// want, and then feed results from running a TensorFlow model into the
// processing method. The timestamp for each subsequent call should be
// increasing from the previous, since the class is designed to process a stream
// of data over time.

class RecognizeCommands {
 public:
  // labels should be a list of the strings associated with each one-hot score.
  // The window duration controls the smoothing. Longer durations will give a
  // higher confidence that the results are correct, but may miss some commands.
  // The detection threshold has a similar effect, with high values increasing
  // the precision at the cost of recall. The minimum count controls how many
  // results need to be in the averaging window before it's seen as a reliable
  // average. This prevents erroneous results when the averaging window is
  // initially being populated for example. The suppression argument disables
  // further recognitions for a set time after one has been triggered, which can
  // help reduce spurious recognitions.

  // ------------------------RecognizeCommands-------------------------------
  
  // recognize_commands.h --> smooths predictions over time 
  explicit RecognizeCommands(int32_t average_window_duration_ms = 1000, // average_window_duration_ms -> how long to average results over (ms)
                             uint8_t detection_threshold = 200, // detection_threshold  -> minimum confidence score for valid prediction
                             int32_t suppression_ms = 1500,     // suppression_ms       -> time (ms) to suppress duplicate detections
                             int32_t minimum_count = 3);        // minimum_count        -> minimum results before making decisions

  // ------------------------ProcessLatestResults-------------------------------

  // recognize_commands.h --> processes ML results and decides the command  
  // Call this with the results of running a model on sample data.
  TfLiteStatus ProcessLatestResults(const TfLiteTensor* latest_results, 
                                    const int32_t current_time_ms,
                                    const char** found_command, uint8_t* score,
                                    bool* is_new_command);


  // ------------------------PRIV-VARIABLES-------------------------------
 private:
  // Configuration
  int32_t average_window_duration_ms_; // how long (ms) we average results over
  uint8_t detection_threshold_; // how strong a recognition score must be before we accept it
  int32_t suppression_ms_;      // time after a detection when no new command is allowed
  int32_t minimum_count_;       // minimum number of valid results added before deciding

  // Working variables
  PreviousResultsQueue previous_results_; // stores past results for smoothing
  const char* previous_top_label_;        // last detected command
  int32_t previous_top_label_time_;       // timestamp of last detected command
};

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_RECOGNIZE_COMMANDS_H_
