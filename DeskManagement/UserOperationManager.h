#ifndef USEROPERATIONMANAGER_H
#define USEROPERATIONMANAGER_H

#include "UserDataAccess.h"
#include <string>

class UserOperationManager {
private:
    static UserOperationManager* instance; 

    UserOperationManager() {
		UserDataAccess::loadJson("UserData.json");
    } 

public:
    static UserOperationManager* getInstance();

    json* getUserInfo(const string& username);
    json* getUserInfoByUsername(const string& username);
    void updateUserBooking(const string& username, const json& booking);
    void removeUserBooking(const string& username, const string& deskID);
};

#endif 
