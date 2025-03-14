#ifndef GET_DESK_AVAILABILITY_COMMAND_H
#define GET_DESK_AVAILABILITY_COMMAND_H

#include "Command.h"
#include "DeskOperationManager.h"
#include "UserType.h"
#include "DeskType.h"
#include <string>

class GetDeskAvailabilityCommand : public Command {
private:
    string location;
    int floor;
    string block;
    int deskNo;
    DeskType deskType;
    system_clock::time_point date;

public:
    GetDeskAvailabilityCommand(const string& location, int floor,
                    const string& block,
                    int deskNo,
                    DeskType deskType, system_clock::time_point date)
        : location(location), floor(floor), block(block), deskNo(deskNo), deskType(deskType), date(date) {
    }

    void execute() override;
};

#endif 
