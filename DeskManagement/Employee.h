#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "User.h"

class Employee : public User {
public:
    Employee(const string& username, const string& id)
        : User(username, id, UserType::Employee) {
    }

    void displayUserInfo() const override {
        cout << "Employee: " << username << " (ID: " << userID << ")\n";
    }
};

#endif
