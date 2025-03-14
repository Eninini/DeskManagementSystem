#include <iostream>
#include <memory>
#include "Login.h"
#include "UserFactory.h"
#include "Helper.h"
#include "BookDeskCommand.h"
#include "CancelBookingCommand.h"
#include "GetDeskAvailabilityCommand.h"
#include "ShowBookingCommand.h"

using namespace std;

void showMenu(unique_ptr<User>& user) {
    while (true) {
        cout << "\n1. Book Flexible Desk\n"
            << "2. Show Desk Availability\n"
            << "3. Cancel Booking\n"
            << "4. Show My Bookings\n";

        if (user->getUserType() == UserType::Manager) {
            cout << "5. Assign Permanent Desk\n"
                << "6. Get Available Permanent Desks\n";
        }

        cout << "0. Logout\n"
            << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) { 
            string deskID, startDateStr;
            int noOfDays;

            cout << "Enter Desk ID: ";
            cin >> deskID;
            cout << "Enter Start Date (YYYY-MM-DD): ";
            cin >> startDateStr;
            cout << "Enter No of Days (Max 3): ";
            cin >> noOfDays;

            if (noOfDays > 3) {
                cerr << "Error: Cannot book for more than 3 days.\n";
                continue;
            }

            auto startDate = Helper::formatStringToDate(startDateStr);
            auto endDate = startDate + chrono::hours(24 * noOfDays);

            string assignedTo = user->getUserId();

            user->setCommand(make_unique<BookDeskCommand>(
                deskID, startDate, endDate, user->getUserType(), assignedTo, user->getUserId()));
        }
        else if (choice == 2) { 
            string location, block, deskNoStr;
            int floor, deskNo;
            string dateStr;

            cout << "Enter Date (YYYY-MM-DD): ";
            cin >> dateStr;
            cout << "Enter Location: ";
            cin >> location;
            cout << "Enter Floor: ";
            cin >> floor;
            cin.ignore(); 

            cout << "Enter Block (optional, press Enter to skip): ";
            getline(cin, block); 
            cout << "Enter Desk Number (-1 for any): ";
            getline(cin, deskNoStr);

            if (deskNoStr.empty()) {
                deskNo = -1; 
            }
            else {
                deskNo = stoi(deskNoStr); 
            }
            string todayDate = Helper::formatDateToString(system_clock::now());

            if (dateStr < todayDate) {
                cerr << "Desk availability cannot be checked for past dates.";
                continue;
            }
            auto date = Helper::formatStringToDate(dateStr);
            user->setCommand(make_unique<GetDeskAvailabilityCommand>(
                location, floor, block.empty() ? "" : block, deskNo, DeskType::Flexible, date));
        }
        else if (choice == 3) { 
            string deskID;
            cout << "Enter Desk ID: ";
            cin >> deskID;

            user->setCommand(make_unique<CancelBookingCommand>(
                deskID, user->getUserId(), user->getUserType()));
        }
        else if (choice == 4) { 
            user->setCommand(make_unique<ShowBookingCommand>(user->getUserId()));
        }
        else if (user->getUserType() == UserType::Manager && choice == 5) { 
            string deskID, assignedTo, startDateStr, endDateStr;
            cout << "Enter Desk ID: ";
            cin >> deskID;
            cout << "Enter Assigned Employee ID: "; 
            cin >> assignedTo;
            cout << "Enter Start Date (YYYY-MM-DD): ";
            cin >> startDateStr;
            cout << "Enter End Date (YYYY-MM-DD): ";
            cin >> endDateStr;

            auto startDate = Helper::formatStringToDate(startDateStr);
            auto endDate = Helper::formatStringToDate(endDateStr);

            user->setCommand(make_unique<BookDeskCommand>(
                deskID, startDate, endDate, UserType::Manager, assignedTo, user->getUserId()));
        }
        else if (user->getUserType() == UserType::Manager && choice == 6) {
            string location, block, deskNoStr;
            int floor, deskNo;
            string dateStr;

            cout << "Enter Date (YYYY-MM-DD): ";
            cin >> dateStr;
            cout << "Enter Location: ";
            cin >> location;
            cout << "Enter Floor: ";
            cin >> floor;
            cin.ignore();

            cout << "Enter Block (optional, press Enter to skip): ";
            getline(cin, block);
            cout << "Enter Desk Number (-1 for any): ";
            getline(cin, deskNoStr);

            if (deskNoStr.empty()) {
                deskNo = -1;
            }
            else {
                deskNo = stoi(deskNoStr);
            }

            auto date = Helper::formatStringToDate(dateStr);
            user->setCommand(make_unique<GetDeskAvailabilityCommand>(
                location, floor, block.empty() ? "" : block, deskNo, DeskType::Permanent, date));
        }
      
        user->executeCommand();
    }
}

int main() {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    string employeeID = Login::authenticateUser(username, password);

    if (employeeID.empty()) {
        return 1;
    }

    auto user = UserFactory::createUser(username);

    if (!user) {
        return 1;
    }

    try {
        showMenu(user);
    }
    catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
        return 0;
    }
    catch (...) {
        cerr << "An unknown error occurred during desk booking.\n";
        return 0;
    }

    cout << "Logged out successfully.\n";
    return 0;
}
