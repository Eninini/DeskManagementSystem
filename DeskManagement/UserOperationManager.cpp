#include "UserOperationManager.h"

UserOperationManager* UserOperationManager::instance = nullptr;

UserOperationManager* UserOperationManager::getInstance() {
    if (!instance) {
        instance = new UserOperationManager();
		UserDataAccess::loadJson("UserData.json");
    }
    return instance;
}

json* UserOperationManager::getUserInfo(const string& employeeId) {
    UserDataAccess::cleanExpiredBookings();
    return UserDataAccess::getUserByEmployeeId(employeeId);
}

json* UserOperationManager::getUserInfoByUsername(const string& username) {
    UserDataAccess::cleanExpiredBookings();
    return UserDataAccess::getUserByUsername(username);
}

void UserOperationManager::updateUserBooking(const string& employeeId, const json& booking) {
    UserDataAccess::updateUserBooking(employeeId, booking);
}

void UserOperationManager::removeUserBooking(const string& employeeId, const string& deskID) {
    UserDataAccess::removeUserBooking(employeeId, deskID);
}
