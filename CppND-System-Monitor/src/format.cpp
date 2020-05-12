#include <string>

#include "format.h"

#define HOUR 3600
#define MIN 60

using std::string;
using std::to_string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 


    int hour, minute, second;
    string hh, mm, ss;
    hour = seconds / HOUR;
    second = seconds % HOUR;
    minute = second / MIN;
    second %= MIN;

    hh = (hour < 10) ? ("0" + to_string(hour)) : to_string(hour);
    mm = (minute < 10) ? ("0" + to_string(minute)) : to_string(minute);
    ss = (second < 10) ? ("0" + to_string(second)) : to_string(second);
    return hh + ":" + mm + ":" + ss;
}