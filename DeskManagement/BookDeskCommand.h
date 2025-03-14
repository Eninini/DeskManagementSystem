#ifndef BOOKDESKCOMMAND_H
#define BOOKDESKCOMMAND_H

#include "Command.h"
#include "DeskOperationManager.h"
#include "UserType.h"
#include "DeskType.h"
#include <string>

class BookDeskCommand : public Command {
private:
    string deskID, assignedToUserId, AssignerUserId;
    system_clock::time_point startDate, endDate;
    int duration;
    UserType userType;

public:
    BookDeskCommand(const string& deskID,
        const system_clock::time_point& startDate, const system_clock::time_point& endDate, UserType userType, const string& assignedToUserId, const string& AssignerUserId)
        : deskID(deskID), assignedToUserId(assignedToUserId), AssignerUserId(AssignerUserId),
        startDate(startDate), endDate(endDate), duration(duration), userType(userType) {
    }

    void execute() override;
};

#endif
