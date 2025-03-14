#ifndef USERDATAACCESS_H
#define USERDATAACCESS_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include "nlohmann/json.hpp"
#include "Helper.h"

using namespace std;
using json = nlohmann::json;

class UserDataAccess {
private:
    static json userData; 
    static unordered_map<string, json*> userByEmployeeID; 
    static unordered_map<string, json*> userByUsername;  
    static mutex userMutex;

    static void buildUserMap();

public:
    static void loadJson(const string& filename);
    static void saveJson(const string& filename);
    static json* getUserByEmployeeId(const string& employeeID);
    static json* getUserByUsername(const string& username);
    static void updateUserBooking(const string& employeeID, const json& booking);
    static void removeUserBooking(const string& employeeID, const string& deskID);
    static void cleanExpiredBookings();
};

#endif
