#ifndef SHOWBOOKINGCOMMAND_H
#define SHOWBOOKINGCOMMAND_H

#include "Command.h"
#include "UserDataAccess.h"
#include <string>
#include <iostream>

class ShowBookingCommand : public Command {
private:
    string employeeID;

public:
    explicit ShowBookingCommand(const string& employeeID) : employeeID(employeeID) {}

    void execute() override;
};

#endif 
