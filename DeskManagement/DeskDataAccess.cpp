#include "DeskDataAccess.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

json DeskDataAccess::deskData;
unordered_map<string, json*> DeskDataAccess::permanentDesks;
unordered_map<string, json*> DeskDataAccess::flexibleDesks;
bool DeskDataAccess::isDataLoaded = false;

mutex DeskDataAccess::dataMutex;

// Load JSON from file and populate desk maps
void DeskDataAccess::loadJson(const string& filename) {
    if (!isDataLoaded) {
        ifstream file(filename);
        if (file.is_open()) {
            file >> deskData;
            file.close();
            buildDeskMaps();
            isDataLoaded = true;
        }
        else {
            cerr << "Error opening file!\n";
        }
    }
}

// Save JSON to file
void DeskDataAccess::saveJson(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << deskData.dump(4);
        file.close();
    }
    else {
        cerr << "Error writing to file!\n";
    }
}

// Build desk maps
void DeskDataAccess::buildDeskMaps() {
    permanentDesks.clear();
    flexibleDesks.clear();

    for (auto& location : deskData["locations"]) {
        string locationName = location["location"];

        for (auto& floor : location["floors"]) {
            int floorNumber = floor["floor"];

            for (auto& block : floor["blocks"]) {
                string blockName = block["block"];

                for (auto& desk : block["desks"]) {
                    string deskID = desk["deskID"];

                    // Ensure 'bookings' field exists as an array
                    if (!desk.contains("bookings") || !desk["bookings"].is_array()) {
                        desk["bookings"] = json::array();
                    }

                    // Add extra fields for convenience
                    desk["location"] = locationName;
                    desk["floor"] = floorNumber;
                    desk["block"] = blockName;

                    if (desk["isPermanent"]) {
                        permanentDesks[deskID] = &desk;
                    }
                    else {
                        flexibleDesks[deskID] = &desk;
                    }
                }
            }
        }
    }
}

// Get desk by ID
json* DeskDataAccess::getDeskByID(const string& deskID) {
    if (permanentDesks.find(deskID) != permanentDesks.end()) {
        return permanentDesks[deskID];
    }
    if (flexibleDesks.find(deskID) != flexibleDesks.end()) {
        return flexibleDesks[deskID];
    }
    return nullptr;
}

// Check if a desk is available for a given date range
bool DeskDataAccess::isDeskAvailable(const string& deskID, const string& startDate, const string& endDate) {
    json* deskInfo = getDeskByID(deskID);
    if (!deskInfo) return false;

    for (const auto& booking : (*deskInfo)["bookings"]) {
        string existingStart = booking["startDate"];
        string existingEnd = booking["endDate"];

        if (!(endDate < existingStart || startDate > existingEnd)) {
            return false; // Overlapping booking found
        }
    }
    return true;
}

// Assign a desk to an employee (with date range check)
bool DeskDataAccess::assignDesk(DeskType deskType, const string& deskID, const string& employeeID, const system_clock::time_point& startDate, const system_clock::time_point& endDate) {
    unique_lock<mutex> lock(dataMutex);
    json* deskInfo = getDeskByID(deskID);
    if (!deskInfo) return false;

    string startDateStr = Helper::formatDateToString(startDate);
    string endDateStr = Helper::formatDateToString(endDate);
    string todayDate = Helper::formatDateToString(system_clock::now());
    //string startDate_ = Helper::formatDateToString(startDate);
    if (startDateStr < todayDate) {
        cerr << "Enter valid date\n";
        return false;
    }
    // Check availability before assigning
    if (!isDeskAvailable(deskID, startDateStr, endDateStr)) {
        cout << "Desk is already booked for the selected dates.\n";
        return false;
    }

    (*deskInfo)["bookings"].push_back({
        {"employeeID", employeeID},
        {"startDate", startDateStr},
        {"endDate", endDateStr}
        });

    return true;
}

// Cancel assigned desk
bool DeskDataAccess::cancelDesk(const string& userId, const string& deskID, const string& startDate, const string& endDate) {
    unique_lock<mutex> lock(dataMutex);
    json* deskInfo = getDeskByID(deskID);
    auto& deskBookings = (*deskInfo)["bookings"];
    deskBookings.erase(remove_if(deskBookings.begin(), deskBookings.end(), [&](const json& booking) {
        return booking["employeeID"] == userId && booking["startDate"] == startDate && booking["endDate"] == endDate;
        }), deskBookings.end());
    return true;
}

// Remove expired bookings
void DeskDataAccess::removeExpiredBookings() {
    unique_lock<mutex> lock(dataMutex);
    string todayDate = Helper::formatDateToString(system_clock::now());

    auto cleanExpired = [&](unordered_map<string, json*>& desks) {
        for (auto& pair : desks) {
            json& desk = *pair.second;
            auto& bookings = desk["bookings"];

            // Remove past bookings
            bookings.erase(
                remove_if(bookings.begin(), bookings.end(), [&](const json& booking) {
                    return booking["endDate"] < todayDate;
                    }),
                bookings.end()
            );
        }
        };

    cleanExpired(permanentDesks);
    cleanExpired(flexibleDesks);
}

// Get desks availability
json DeskDataAccess::getDesksAvailability(DeskType deskType, const string& location, int floor, string block, int deskNo, system_clock::time_point date) {
    removeExpiredBookings();
    string dateStr = (date == system_clock::time_point{}) ? Helper::formatDateToString(system_clock::now()) : Helper::formatDateToString(date);

    json result = json::array();

    auto searchAndAdd = [&](const unordered_map<string, json*>& desks) {
        for (const auto& pair : desks) {
            const json& desk = *pair.second;

            bool matchesLocation = (desk["location"] == location);
            bool matchesFloor = (desk["floor"] == floor);
            bool matchesBlock = (block.empty() || desk["block"] == block);
            bool matchesDeskNo = (deskNo == -1 || desk["deskNo"] == deskNo);
            bool isAvailable = isDeskAvailable(desk["deskID"], dateStr, dateStr);

            if (matchesLocation && matchesFloor && matchesBlock && matchesDeskNo && isAvailable) {
                result.push_back(desk);
            }
        }
        };

    if (deskType == DeskType::Permanent || deskType == DeskType::Both) {
        searchAndAdd(permanentDesks);
    }
    if (deskType == DeskType::Flexible || deskType == DeskType::Both) {
        searchAndAdd(flexibleDesks);
    }

    return result;
}
