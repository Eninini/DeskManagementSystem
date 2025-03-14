#include "UserDataAccess.h"
#include <fstream>
#include <iostream>


json UserDataAccess::userData;
unordered_map<string, json*> UserDataAccess::userByEmployeeID;
unordered_map<string, json*> UserDataAccess::userByUsername;

mutex UserDataAccess::userMutex;

void UserDataAccess::loadJson(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        file >> userData;
        file.close();
        buildUserMap(); 
    }
    else {
        cerr << "Error opening file: " << filename << endl;
    }
}

void UserDataAccess::saveJson(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << userData.dump(4);
        file.close();
    }
    else {
        cerr << "Error writing to file: " << filename << endl;
    }
}

void UserDataAccess::buildUserMap() {
    userByEmployeeID.clear();
    userByUsername.clear();

    for (auto& user : userData["users"]) {
        string employeeID = user["employeeID"];
        string username = user["username"];

        userByEmployeeID[employeeID] = &user;
        userByUsername[username] = &user;
    }
}

json* UserDataAccess::getUserByEmployeeId(const string& employeeID) {
    if (userByEmployeeID.count(employeeID)) {
        return userByEmployeeID[employeeID];
    }
    return nullptr;
}

json* UserDataAccess::getUserByUsername(const string& username) {
    if (userByUsername.count(username)) {
        return userByUsername[username];
    }
    return nullptr;
}

void UserDataAccess::updateUserBooking(const string& employeeID, const json& booking) {
    unique_lock<mutex> lock(userMutex);
    json* user = getUserByEmployeeId(employeeID);
    if (user) {
        if (!user->contains("bookings")) {
            (*user)["bookings"] = json::array();
        }
        (*user)["bookings"].push_back(booking);
    }
}

void UserDataAccess::removeUserBooking(const string& employeeID, const string& deskID) {
    unique_lock<mutex> lock(userMutex);
    json* userInfo = getUserByEmployeeId(employeeID);

    auto& userBookings = (*userInfo)["bookings"];
    auto bookingIt = find_if(userBookings.begin(), userBookings.end(), [&](const json& booking) {
        return booking["deskID"] == deskID;
        });

    string bookingStartDate = (*bookingIt)["startDate"];
    string bookingEndDate = (*bookingIt)["endDate"];

    if (userInfo) {
        auto& assignedToBookings = (*userInfo)["bookings"];
        assignedToBookings.erase(remove_if(assignedToBookings.begin(), assignedToBookings.end(), [&](const json& booking) {
            return booking["deskID"] == deskID &&
                booking["startDate"] == bookingStartDate &&
                booking["endDate"] == bookingEndDate;
            }), assignedToBookings.end());
    }
}

void UserDataAccess::cleanExpiredBookings() {
    unique_lock<mutex> lock(userMutex);
    string currentDateStr = Helper::formatDateToString(system_clock::now());
    system_clock::time_point currentDate = Helper::formatStringToDate(currentDateStr);

    for (auto& user : userData["users"]) {
        if (user.contains("bookings")) {
            json& bookings = user["bookings"];

            bookings.erase(remove_if(bookings.begin(), bookings.end(), [&](const json& booking) {
                system_clock::time_point endDate = Helper::formatStringToDate(booking["endDate"]);
                return endDate < currentDate;
                }), bookings.end());
        }
    }
}
