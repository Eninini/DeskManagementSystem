#include "ShowBookingCommand.h"

void ShowBookingCommand::execute() {
    try {
        json* user = UserOperationManager::getInstance()->getUserInfo(employeeID);

        if (!user) {
            cout << "Error: Employee with ID " << employeeID << " not found." << endl;
            return;
        }

        if (!user->contains("bookings") || (*user)["bookings"].empty()) {
            cout << "No bookings found for Employee ID: " << employeeID << endl;
            return;
        }

        vector<string> selfBookings;
        vector<string> assignedBookings;

        for (const auto& booking : (*user)["bookings"]) {
            stringstream ss;
            ss << "Desk ID: " << booking["deskID"]
                << ", Start Date: " << booking["startDate"]
                << ", End Date: " << booking["endDate"];


            if (booking["assignedTo"] == employeeID) {
                selfBookings.push_back(ss.str());
            }
            else {
                ss << ", Assigned To: " << booking["assignedTo"];
                assignedBookings.push_back(ss.str());
            }
        }

        cout << "Bookings for Employee ID: " << employeeID << endl;

        if (!selfBookings.empty()) {
            cout << "\nDesks assigned to this employee:\n";
            for (const auto& entry : selfBookings) {
                cout << entry << endl;
            }
        }

        if (!assignedBookings.empty()) {
            cout << "\nDesks assigned by this employee to others:\n";
            for (const auto& entry : assignedBookings) {
                cout << entry << endl;
            }
        }
    }
    catch (const std::exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }
    catch (...) {
        cerr << "An unknown error occurred during desk booking.\n";
    }
}
