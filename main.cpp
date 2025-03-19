/*
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	*/

// main.cpp : Defines the entry point for the application.

#include "DBFunctions.hpp"

SQLHENV			hEnv;
SQLHDBC			hDbc;
SQLHSTMT		hStmt;
bool			isTestServer;

//Variables for Device
std::wstring    deviceNumber;
std::wstring    serialTag;
std::wstring    deviceModelId;
std::wstring    currentlyIssued;
std::wstring    purchaseDate;
std::wstring    deviceCost;
std::wstring    issueDate;
std::wstring    firstUser;
std::wstring    operatingSystem;


//Variables for employees
std::wstring    employeeName;
std::wstring    employeeTitle;
std::wstring    employeeLocation;
std::wstring    employeeEmailAddress;
std::wstring    employeePhoneNumber;
std::wstring    employeeExtension;
std::wstring    employeeID;

//Loop breakers
char            yesOrNo;
bool            endUnassignLoop;
int             checkVar;

int main() {
	/*
	
		Main function used for entering the program.

	*/

	
	hEnv = NULL;
	hDbc = NULL;
	hStmt = NULL;

	//Run the connect database function.
	int connectResult = connectDatabase(hEnv, hDbc, hStmt);

	// End the program if the connection fails
	if (connectResult != 0) {
		return -1;
	}

	// Main loop of the program. When this loop is broken, the program will exit.
	bool queryFinished = false;

	while (!queryFinished) {

		std::cout << std::endl;

		// Prompt the user to choose a selection.
		std::cout << "What would you like to do?" << std::endl << std::endl;
		std::cout << "Press 1 to assign a device to an employee." << std::endl << std::endl;
		std::cout << "Press 2 to unassign a device." << std::endl << std::endl;
		std::cout << "Press 3 to add a new device." << std::endl << std::endl;
		std::cout << "Press 4 to add an employee." << std::endl << std::endl;
		std::cout << "Press 5 to remove an employee." << std::endl << std::endl;
		std::cout << "Press 6 to remove a device." << std::endl << std::endl;
		std::cout << "Press 0 to read the last device entered." << std::endl << std::endl;
		std::cout << std::endl << "Press q or Q to quit." << std::endl << std::endl;

		//clear error flags and stream buffer
		std::cin.clear();

		// User inputs selection
		char queryChoice;
		std::cin >> queryChoice;

		// Main menu
		switch (queryChoice) {

			case '1':
				assignDevice(hStmt);
				break;

			case '2':
				unassignDevice(hStmt);
				break;

			case '3':
				newDevice(hStmt);
				break;

			case '4':
				addEmployee(hStmt);
				break;

			case '5':
				removeEmployee(hStmt);
				break;

			case '6':
				removeDevice(hStmt);
				break;
				
			case '0':
				readLastDevice();
				enterKey();
				break;
				
			case 'q':
				queryFinished = true;
				break;

			case 'Q':
				queryFinished = true;
				break;

			default:
				//if invalid input is entered
				std::cout << std::endl << "Please select a valid option." << std::endl << std::endl;
				break;
		}
	}
	return 0;
}
