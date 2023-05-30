#include "format.h"

#include <string>
#include <iomanip>

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds ) { 
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    long s = seconds % 3600 % 60;
    // Before it can show seconds greater that 59. To solve this:
    // When the output from 0 to 9 it is only one digit so the old digit (rightmost digit) is kept unchanged or cleared (the last value in this case 59) so 9 is still there.
    // What you need to do is make sure each variable output will be in 2-digit (hours, seconds and minutes) and adding leading zero,
    
    std::ostringstream stream;
    stream << std::setw(2) << std::setfill('0') << hours << ":" 
     << std::setw(2) << std::setfill('0') << minutes << ":"
     << std::setw(2) << std::setfill('0') << s;

   return  stream.str();

   /** Alternative implementation:
    std::chrono::seconds seconds{s};
    // return std::chrono::format("%T", seconds); // in C++20
    
    std::chrono::hours hours =
    std::chrono::duration_cast<std::chrono::hours>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);
    std::chrono::minutes minutes =
    std::chrono::duration_cast<std::chrono::minutes>(seconds);
    
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

    std::stringstream ss{};

    ss << std::setw(2) << std::setfill('0') << hours.count()   // HH
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << minutes.count()   // MM
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << seconds.count();  // SS

    return ss.str();
   */
}