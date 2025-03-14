#ifndef CANCELBOOKINGCOMMAND_H
#define CANCELBOOKINGCOMMAND_H

#include "Command.h"
#include "DeskOperationManager.h"
#include "UserType.h"
#include "DeskType.h"
#include <string>

class CancelBookingCommand : public Command {
private:
    string deskID, userId;
    UserType userType;

public:
    CancelBookingCommand(const string& deskID,
        const string& userId, UserType userType)
        : deskID(deskID), userId(userId), userType(userType) {
    }

    void execute() override;
};

#endif 
