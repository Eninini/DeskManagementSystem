#include "CancelBookingCommand.h"
#include <iostream>

void CancelBookingCommand::execute() {
    try {
        json* deskInfo = FlexDeskManager::getInstance()->getDeskDetailByID(deskID);
        if (!deskInfo) {
            deskInfo = PermanentDeskManager::getInstance()->getDeskDetailByID(deskID);
        }

        if (!deskInfo) {
            cerr << "Error: Desk not found." << endl;
            return;
        }

        DeskType type = (*deskInfo)["isPermanent"].get<bool>() ? DeskType::Permanent : DeskType::Flexible;

        if (type == DeskType::Permanent && userType == UserType::Employee) {
            cerr << "Error: Employees cannot cancel permanent desks." << endl;
            return;
        }

        json* userInfo = UserOperationManager::getInstance()->getUserInfo(userId);
      
        if (!userInfo) {
            cerr << "Error: User not found." << endl;
            return;
        }

        auto& userBookings = (*userInfo)["bookings"];
        auto bookingIt = find_if(userBookings.begin(), userBookings.end(), [&](const json& booking) {
            return booking["deskID"] == deskID;
            });

        if (bookingIt == userBookings.end()) {
            cerr << "Error: User does not have this desk booked." << endl;
            return;
        }

        string bookingStartDate = (*bookingIt)["startDate"];
        string bookingEndDate = (*bookingIt)["endDate"];
        string assignedToUser = (*bookingIt)["assignedTo"];
        string assignedByUser = (*bookingIt)["assignedBy"];

        bool success = false;
        if (type == DeskType::Flexible) {
            success = FlexDeskManager::getInstance()->cancelDesk(userId, deskID, bookingStartDate, bookingEndDate);
        }
        else if (type == DeskType::Permanent) {
            success = PermanentDeskManager::getInstance()->cancelDesk(userId, deskID, bookingStartDate, bookingEndDate);
        }

        if (success) {
            UserOperationManager::getInstance()->removeUserBooking(assignedToUser, deskID);
            if (assignedByUser != assignedToUser) { //for flex desks, assigned by is same as assigned to
                UserOperationManager::getInstance()->removeUserBooking(assignedByUser, deskID);
            }
            cout << "Desk " << deskID << " successfully canceled for user " << userId << ".\n";
        }
        else {
            cerr << "Error: Cancellation failed for desk " << deskID << ".\n";
        }
    }
    catch (const  exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }
    catch (...) {
        cerr << "An unknown error occurred during desk booking.\n";
    }
}
