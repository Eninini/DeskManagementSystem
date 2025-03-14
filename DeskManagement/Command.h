#ifndef COMMAND_H
#define COMMAND_H

#include "FlexDeskManager.h"
#include "PermanentDeskManager.h"
#include "UserOperationManager.h"

class Command {
protected:
    FlexDeskManager* flexDeskMgr = FlexDeskManager::getInstance();
    PermanentDeskManager* permDeskMgr = PermanentDeskManager::getInstance();
	UserOperationManager* userOpMgr = UserOperationManager::getInstance();

public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

#endif 