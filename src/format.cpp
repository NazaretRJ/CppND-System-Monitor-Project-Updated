#include "format.h"

#include <string>

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds ) { 
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    long s = seconds % 3600 % 60;
    
   return  std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(s);
}