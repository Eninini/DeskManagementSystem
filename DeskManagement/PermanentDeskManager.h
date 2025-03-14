#ifndef PERMANENT_DESK_MANAGER_H
#define PERMANENT_DESK_MANAGER_H

#include "DeskOperationManager.h"

class PermanentDeskManager : public DeskOperationManager {
private:
    static PermanentDeskManager* _instance;
    PermanentDeskManager() = default;

public:
    static PermanentDeskManager* getInstance();

    json* getDeskDetailByID(const string& deskID);
    json getDeskAvailability(const string& location, int floor, const string& block, int deskNo, system_clock::time_point date);
    bool bookDesk(const string& deskID, const string& employeeID, const system_clock::time_point& startDate, const system_clock::time_point& endDate);
    bool cancelDesk(const string& userId, const string& deskID, const string& startDate, const string& endDate);
};

#endif 
