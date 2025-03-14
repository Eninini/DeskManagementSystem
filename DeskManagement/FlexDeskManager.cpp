#include "FlexDeskManager.h"

FlexDeskManager* FlexDeskManager::_instance = nullptr;

FlexDeskManager* FlexDeskManager::getInstance() {
    if (!_instance) {
        _instance = new FlexDeskManager();      //loads the json into the data strcutures
    }
    return _instance;
}

json* FlexDeskManager::getDeskDetailByID(const string& deskID) {
    return DeskDataAccess::getDeskByID(deskID);
}

json FlexDeskManager::getDeskAvailability(const string& location, int floor, const string& block, int deskNo, system_clock::time_point date) {
    return DeskDataAccess::getDesksAvailability(DeskType::Flexible, location, floor, block, deskNo, date);
}

bool FlexDeskManager::bookDesk(const string& deskID, const string& employeeID, const system_clock::time_point& startDate, const system_clock::time_point& endDate) {
  //  string todayDate = Helper::formatDateToString(system_clock::now());
   // string startDate_ = Helper::formatDateToString(startDate);
    if (duration_cast<duration<int, ratio<86400>>>(endDate - startDate).count() > 3 ) {
        cout << "Could not book desk\n";
        return false;
    }
    return DeskDataAccess::assignDesk(DeskType::Flexible, deskID, employeeID, startDate, endDate);
}

bool FlexDeskManager::cancelDesk(const string& userId, const string& deskID, const string& startDate, const string& endDate) {
	return DeskDataAccess::cancelDesk(userId, deskID, startDate, endDate);
}
