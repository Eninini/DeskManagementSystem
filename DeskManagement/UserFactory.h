#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "User.h"
#include "Employee.h"
#include "Manager.h"
#include "UserOperationManager.h"
#include <memory>

class UserFactory {
public:
    static unique_ptr<User> createUser(const string& username) {
        json* userInfo = UserOperationManager::getInstance()->getUserInfoByUsername(username);

        if (!userInfo) {
            cerr << "Error: User not found.\n";
            return nullptr;
        }

        string userID = (*userInfo)["employeeID"];
        string roleStr = (*userInfo)["role"].get<string>();

        UserType userType;
        if (roleStr == "Manager") {
            userType = UserType::Manager;
            return make_unique<Manager>(username, userID);
        }
        else if (roleStr == "Employee") {
            userType = UserType::Employee;
            return make_unique<Employee>(username, userID);
        }
        else {
            cerr << "Error: Invalid user type.\n";
            return nullptr;
        }
    }
};

#endif
