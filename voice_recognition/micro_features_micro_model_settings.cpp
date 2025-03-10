
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== micro_features_micro_model_settings.cpp ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // stores the labels for the words which the AI recognizes
    
//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#include "micro_features_micro_model_settings.h"

//---------------------------------------------------------------------WORDS-RECOGNIZED-LABELS----------------------------------------------------------------------------------------------

// the strings inside here can be modified to suit what command words are needed
// of course data needs to be fed into the loop to recognise it first
const char* kCategoryLabels[kCategoryCount] = {
    "silence",
    "unknown",
    "yes",
    "no",
};
