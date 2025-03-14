#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"

class Manager : public Employee {
public:
    Manager(const string& username, const string& id)
        : Employee(username, id) {
        userType = UserType::Manager;
    }

    void displayUserInfo() const override {
        cout << "Manager: " << username << " (ID: " << userID << ")\n";
    }
};

#endif
