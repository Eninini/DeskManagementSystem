#include "Login.h"
#include "UserOperationManager.h"
#include <iostream>

string Login::authenticateUser(const string& username, const string& password) {
    json* user = UserOperationManager::getInstance()->getUserInfoByUsername(username);

    if (user && (*user)["password"] == password) {
        return (*user)["employeeID"];
    }

    cerr << "Invalid username or password.\n";
    return "";
}
