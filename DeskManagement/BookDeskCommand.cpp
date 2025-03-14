#include "BookDeskCommand.h"
#include <iostream>

void BookDeskCommand::execute() {
    try {
        json* deskInfo = FlexDeskManager::getInstance()->getDeskDetailByID(deskID);

        if (!deskInfo) {
            deskInfo = PermanentDeskManager::getInstance()->getDeskDetailByID(deskID);
        }

        if (deskInfo) {
            DeskType type = (*deskInfo)["isPermanent"].get<bool>() ? DeskType::Permanent : DeskType::Flexible;

            if (type == DeskType::Permanent && userType == UserType::Employee) {
                cerr << "Error: Employees cannot book permanent desks.\n";
                return;
            }

            json* assignedUserInfo = UserOperationManager::getInstance()->getUserInfo(assignedToUserId);
            if (assignedUserInfo && assignedUserInfo->contains("bookings") && (*assignedUserInfo)["bookings"].is_array()) {
                for (const auto& booking : (*assignedUserInfo)["bookings"]) {
                    auto existingStart = Helper::formatStringToDate(booking["startDate"]);
                    auto existingEnd = Helper::formatStringToDate(booking["endDate"]);
                    DeskType existingType = (booking["deskType"] == "Permanent") ? DeskType::Permanent : DeskType::Flexible;
                    string AssignedTo = booking["assignedTo"];

                    // Check if the user already has a booking of the same type within the requested period
                    if (AssignedTo == assignedToUserId && !(endDate < existingStart || startDate > existingEnd)) {
                        cerr << "Booking failed: User " << assignedToUserId
                            << " already has a desk booked during this period.\n";
                        return;
                    }
                }
            }

            bool success = false;
            if (type == DeskType::Flexible) {
                success = FlexDeskManager::getInstance()->bookDesk(deskID, assignedToUserId, startDate, endDate);
            }
            else if (type == DeskType::Permanent) {
                success = PermanentDeskManager::getInstance()->bookDesk(deskID, assignedToUserId, startDate, endDate);
            }

            if (success) {
                json booking = {
                    {"deskID", deskID},
                    {"startDate", Helper::formatDateToString(startDate)},
                    {"endDate", Helper::formatDateToString(endDate)},
                    {"assignedTo", assignedToUserId},
                    {"deskType", type == DeskType::Permanent ? "Permanent" : "Flexible"},
                    {"assignedBy", AssignerUserId}
                };

                UserOperationManager::getInstance()->updateUserBooking(assignedToUserId, booking);

                if (assignedToUserId != AssignerUserId) {
                    UserOperationManager::getInstance()->updateUserBooking(AssignerUserId, booking);
                }

                cout << "Desk " << deskID << " successfully booked.\n";
            }
            else {
                cerr << "Booking failed for desk " << deskID << "\n";
            }
            return;
        }

        cerr << "Booking failed: Desk " << deskID << " not found.\n";
    }
    catch (const std::exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }
    catch (...) {
        cerr << "An unknown error occurred during desk booking.\n";
    }
}
