#ifndef FLEX_DESK_MANAGER_H
#define FLEX_DESK_MANAGER_H

#include "DeskOperationManager.h"
#include <iostream>

class FlexDeskManager : public DeskOperationManager {
private:
    static FlexDeskManager* _instance;
    FlexDeskManager() = default;

public:
    static FlexDeskManager* getInstance();

    json* getDeskDetailByID(const string& deskID);
    json getDeskAvailability(const string& location, int floor, const string& block, int deskNo, system_clock::time_point date);
    bool bookDesk(const string& deskID, const string& employeeID, const system_clock::time_point& startDate, const system_clock::time_point& endDate);
    bool cancelDesk(const string& userId, const string& deskID, const string& startDate, const string& endDate);
};

#endif 
