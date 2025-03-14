#ifndef DESKDATAACCESS_H
#define DESKDATAACCESS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <mutex>
#include <nlohmann/json.hpp>
#include "DeskType.h"
#include "Helper.h"

using namespace std;
using json = nlohmann::json;
using namespace std::chrono;


class DeskDataAccess {
private:
    static json deskData;
    static unordered_map<string, json*> permanentDesks;
    static unordered_map<string, json*> flexibleDesks;
    static bool isDataLoaded;
    static mutex dataMutex;

    static void buildDeskMaps();
    static bool isDeskAvailable(const string& deskID, const string& startDate, const string& endDate);
    static void removeExpiredBookings();

public:
    static void loadJson(const string& filename);
    static void saveJson(const string& filename);

    static json* getDeskByID(const string& deskID);
    static json getDesksAvailability(DeskType deskType, const string& location, int floor, string block, int deskNo, system_clock::time_point date);

    static bool assignDesk(DeskType deskType, const string& deskID, const string& employeeID, const system_clock::time_point& startDate, const system_clock::time_point& endDate);
    static bool cancelDesk(const string& userId, const string& deskID, const string& startDate, const string& endDate);
};

#endif 
