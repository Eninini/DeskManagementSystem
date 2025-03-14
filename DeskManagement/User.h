#ifndef USER_H
#define USER_H

#include "Command.h"
#include "UserType.h"
#include <memory>
#include <string>
#include <iostream>

class User {
protected:
    string username;
    string userID;
    UserType userType;
    unique_ptr<Command> command;

public:
    User(const string& username, const string& id, UserType type)
        : username(username), userID(id), userType(type) {
    }

    virtual ~User() = default;

    void setCommand(unique_ptr<Command> cmd) {
        command = move(cmd);
    }

    void executeCommand() {
        if (command) {
            command->execute();
        }
        else {
            cerr << "No command assigned to user " << userID << "\n";
        }
    }

    string getUsername() const { return username; }
    string getUserId() const { return userID; }
    UserType getUserType() const { return userType; }

    virtual void displayUserInfo() const = 0;
};

#endif 
