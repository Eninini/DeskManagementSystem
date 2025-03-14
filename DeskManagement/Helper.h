#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

using namespace std;
using namespace std::chrono;

class Helper {
public:
    static string formatDateToString(const system_clock::time_point& date);

    static system_clock::time_point formatStringToDate(const string& dateStr);
};

#endif
