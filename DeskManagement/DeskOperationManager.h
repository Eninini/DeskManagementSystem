#ifndef DESK_OPERATION_MANAGER_H
#define DESK_OPERATION_MANAGER_H

#include "DeskDataAccess.h"

class DeskOperationManager {
protected:
    DeskOperationManager() { DeskDataAccess::loadJson("DeskData.json"); }

};

#endif
