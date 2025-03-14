#include <iostream>
#include <sstream>
#include <iomanip>
#include "Helper.h"

string Helper::formatDateToString(const system_clock::time_point& date) {
    try {
        time_t time = system_clock::to_time_t(date);
        struct tm tmDate;
        gmtime_s(&tmDate, &time);

        stringstream ss;
        ss << put_time(&tmDate, "%Y-%m-%d");
        return ss.str();
    }
    catch (const std::exception& e) {
        throw runtime_error("Error formatting date to string: " + string(e.what()));
    }
    catch (...) {
        throw runtime_error("Unknown error occurred while formatting date to string.");
    }
}

system_clock::time_point Helper::formatStringToDate(const string& dateStr) {
    try {
        struct tm tmDate = {};
        stringstream ss(dateStr);
        ss >> get_time(&tmDate, "%Y-%m-%d");

        if (ss.fail()) {
            throw invalid_argument("Invalid date format: " + dateStr);
        }

        return system_clock::from_time_t(_mkgmtime(&tmDate));
    }
    catch (const std::exception& e) {
        throw runtime_error("Error formatting string to date: " + string(e.what()));
    }
    catch (...) {
        throw runtime_error("Unknown error occurred while formatting string to date.");
    }
}
