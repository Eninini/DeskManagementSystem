#include "GetDeskAvailabilityCommand.h"
#include <iostream>

void GetDeskAvailabilityCommand::execute() {
	try {
		json result;
		if (deskType == DeskType::Permanent) {
			result = PermanentDeskManager::getInstance()->getDeskAvailability(location, floor, block, deskNo, date);
		}
		else {
			result = FlexDeskManager::getInstance()->getDeskAvailability(location, floor, block, deskNo, date);
		}
		cout << "Desk Availability: " << result.dump(4) << endl;
	}
	catch (const std::exception& e) {
		cerr << "An error occurred: " << e.what() << endl;
	}
	catch (...) {
		cerr << "An unknown error occurred during desk booking.\n";
	}
}
