#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include "UserDataAccess.h"

class Login {
public:
    static string authenticateUser(const std::string& username, const std::string& password);
};

#endif 
