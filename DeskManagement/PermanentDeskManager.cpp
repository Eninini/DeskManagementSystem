#include "PermanentDeskManager.h"

PermanentDeskManager* PermanentDeskManager::_instance = nullptr;

PermanentDeskManager* PermanentDeskManager::getInstance() {
    if (!_instance) {
        _instance = new PermanentDeskManager();
    }
    return _instance;
}

json* PermanentDeskManager::getDeskDetailByID(const string& deskID) {
    return DeskDataAccess::getDeskByID(deskID);
}

json PermanentDeskManager::getDeskAvailability(const string& location, int floor, const string& block, int deskNo, system_clock::time_point date) {
    return DeskDataAccess::getDesksAvailability(DeskType::Permanent, location, floor, block, deskNo, date);
}

bool PermanentDeskManager::bookDesk(const string& deskID, const string& employeeID, const system_clock::time_point& startDate, const system_clock::time_point& endDate) {
    return DeskDataAccess::assignDesk(DeskType::Permanent, deskID, employeeID, startDate, endDate);
}

bool PermanentDeskManager::cancelDesk(const string& userId, const string& deskID, const string& startDate, const string& endDate) {
    return PermanentDeskManager::cancelDesk(userId, deskID, startDate, endDate);
}
