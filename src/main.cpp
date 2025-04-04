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

#include "..\include\DBFunctions.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	/*

		Main function used for entering the program.

	*/

	
	//Run the connect database function.
	std::cout << "ENTER '1' FOR TEST SERVER. ENTER '2' FOR LIVE SERVER. FOR COPYRIGHT AND LICENSE INFORMATION ENTER 'C'." << std::endl;
	char dbChoice;
	std::cin >> dbChoice;

	if (dbChoice == 'C' or dbChoice == 'c') 
	{
		std::cout << "This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU Affero General Public License for more details. You should have received a copy of the GNU Affero General Public License along with this program.If not, see <https://www.gnu.org/licenses/>." << std::endl << std::endl;
		std::cout << "ENTER '1' FOR TEST SERVER. ENTER '2' FOR LIVE SERVER." << std::endl;
		std::cin >> dbChoice;
	}

	bool isTestServer = true;
	SQLHENV  hEnv = NULL;
	SQLHDBC  hDbc = NULL;
	SQLHSTMT hStmt = NULL;
	//Get Connection string from config

	int connectResult = connectDatabase(hEnv, hDbc, hStmt, dbChoice, isTestServer);

	// End the program if the connection fails
	if (connectResult != 0) 
	{
		std::cout << "ERROR: DATABSE COULD NOT CONNECT." << std::endl;
		enterKey();
		return -1;
	}

	else 
	{
		std::cout << std::endl << "Connection established." << std::endl << std::endl;
	}

	// Main loop of the program. When this loop is broken, the program will exit.
	bool queryFinished = false;

	while (!queryFinished)
	{

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
		switch (queryChoice)
		{

			// Assign a device to an employee
		case '1':
			/*----------------------------------------------------------------------------------------------------------------------------------------------------------------*\
			|																																								   |
			| The following section consists of a series of while() loops that get input from the user, followed by the assignDevice() function that adds it to the database.  |
			|																																								   |
			\*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		{
			while (true)
			{
				std::wstring deviceNumber;

				// DEVICE NUMBER
				while (true)
				{
					// Declare local variables to pass to assignDevice();
					std::cout << std::endl << "Please scan the device barcode on the device you would like to assign or enter it manually. Enter 'q' or 'Q' to stop." << std::endl;
					std::wcin >> deviceNumber;
					//clear error flags and stream buffer
					std::wcin.clear();
					std::wcin.ignore(10000, '\n');

					if (deviceNumber == L"q" || deviceNumber == L"Q") {
						break;
					}

					// Check if the device name is in the system
					bool isValid = checkValid(hStmt, L"[Devices]", L"[Device_Name]", L"Device_Name", deviceNumber);

					if (isValid)
					{
						break;
					}

					else
					{
						std::wcout << L"There is no device with the name: " << deviceNumber << std::endl;
					}

				}

				if (deviceNumber == L"q" || deviceNumber == L"Q")
				{
					break;
				}

				std::wstring    employeeEmailAddress;

				// EMPLOYEE EMAIL ADDRESS
				while (true)
				{

					std::cout << std::endl << "Empoyee email address you would like to assign to device to. Enter 'q' or 'Q' to stop." << std::endl;
					std::wcin >> employeeEmailAddress;

					//clear error flags and stream buffer
					std::wcin.clear();
					std::wcin.ignore(10000, '\n');

					// Get employee ID from email
					std::wstring employeeID = getIdFromEmail(hStmt, employeeEmailAddress);

					bool isValid = checkValid(hStmt, L"[Employees]", L"[Employee_Email]", L"Employee_ID", employeeID);

					if (isValid)
					{
						break;
					}

					else
					{
						std::wcout << L"There is no employee with the email address: " << employeeEmailAddress << std::endl;
					}
				}

				if (employeeEmailAddress == L"q" || employeeEmailAddress == L"Q")
				{
					break;
				}

				// Get the date of issue
				std::wstring  issueDate;
				std::cout << std::endl << "Please enter the date (DD/MM/YY)." << std::endl;
				std::wcin >> issueDate;
				std::wcin.clear();
				std::wcin.ignore(10000, '\n');

				// Get the first user bool
				std::wstring    firstUser;

				while (firstUser != L"1" && firstUser != L"0")
				{


					std::cout << std::endl << "Is this person the first user of this device? (Y/N)" << std::endl;
					std::wcin >> firstUser;
					std::wcin.clear();
					std::wcin.ignore(10000, '\n');

					if (firstUser == L"Y" || firstUser == L"y")
					{
						firstUser = L"1";
					}

					else if (firstUser == L"N" || firstUser == L"n")
					{
						firstUser = L"0";
					}

					else
					{
						std::cout << std::endl << "Please select a valid option." << std::endl;
					}
				}

				int retcode = assignDevice(hStmt, deviceNumber, employeeEmailAddress, issueDate, firstUser);

				if (retcode == 0) {
					std::cout << std::endl << "SUCCESS." << std::endl << std::endl;
				}

				else {
					std::cout << std::endl << "FAILED." << std::endl << std::endl;
				}
			}
			break;
		}

		// Unassign a device from an employee
		case '2':
		{

			while (true)
			{
				std::wstring deviceNumber;
				std::cout << "\nPlease enter the device's name (NHXX) or scan the barcode with the device's name on it. Enter 'q' or 'Q' to stop." << std::endl;
				std::wcin >> deviceNumber;
				std::wcin.clear();
				std::wcin.ignore(10000, '\n');

				if (deviceNumber == L"q" || deviceNumber == L"Q")
				{
					break;
				}

				int isValid = checkValid(hStmt, L"[Devices]", L"[Device_Name]", L"Device_Name", deviceNumber);

				if (isValid)
				{
					int retcode = unassignDevice(hStmt, deviceNumber);

					if (retcode == 0) {
						std::cout << std::endl << "SUCCESS." << std::endl << std::endl;
					}
					else {
						std::cout << std::endl << "FAILED." << std::endl << std::endl;
					}
					
				}

				else
				{
					std::wcout << L"There is no device with the name: " << deviceNumber << std::endl;
				}

			}
			break;
		}

		// Enter a new device into the database
		case '3':
		{
			char yesOrNo = 'a';

			//Run this loop until the user is satisfied.
			while (yesOrNo != 'y' && yesOrNo != 'Y')
			{
				//reset yesOrNo so that the loop will come back around

				//Gathering inputs, Device_Name, Serial_Tag, Device_Model_ID, Currently_Issued_To, Date_Purchased
				std::wstring deviceNumber;
				std::cout << "\nPlease scan the computer's barcode or enter the computer name." << std::endl;
				std::cin.ignore(80, '\n');
				std::getline(std::wcin, deviceNumber);

				std::wstring serialTag;
				std::cout << "\nPlease scan or enter the computer's serial tag." << std::endl;
				std::getline(std::wcin, serialTag);

				std::wstring deviceModelId;
				std::cout << "\nPlease scan or enter the computer's Device Model ID." << std::endl;
				std::getline(std::wcin, deviceModelId);

				std::wstring purchaseDate;
				std::cout << "\nPlease enter the date in the following format: MM/DD/YYYY" << std::endl;
				std::getline(std::wcin, purchaseDate);
				std::cout << std::endl;

				std::wstring deviceCost;
				std::cout << "\nPlease enter the purchase price of the device." << std::endl;
				std::getline(std::wcin, deviceCost);
				std::cout << std::endl;

				std::wstring operatingSystem;
				std::cout << "\nPlease enter the operating system of the device." << std::endl;
				std::getline(std::wcin, operatingSystem);
				std::cout << std::endl;

				while (true)
				{
					//Reading back the inputs to the user
					std::cout << "\nAre these the correct inputs? \n" << std::endl;

					std::cout << "Computer name: ";
					std::wcout << deviceNumber << std::endl;

					std::cout << "Serial tag: ";

					//Comment out this line if you plan to use a mix of upper and lower case characters
					std::transform(serialTag.begin(), serialTag.end(), serialTag.begin(), ::toupper);

					std::wcout << serialTag << std::endl;

					std::cout << "Device Model ID: ";
					std::wcout << deviceModelId << std::endl;

					std::cout << "Date Purchased: ";
					std::wcout << purchaseDate << std::endl;

					std::cout << "Purchase price: ";
					std::wcout << deviceCost << std::endl;

					std::cout << "Operating System: ";
					std::wcout << operatingSystem << std::endl;

					std::cout << "(Y/N)" << std::endl;

					std::cin >> yesOrNo;

					if (yesOrNo == 'y' || yesOrNo == 'Y') 
					{
						int retcode =newDevice(hStmt, deviceNumber, serialTag, deviceModelId, purchaseDate, deviceCost, operatingSystem, isTestServer);

						if (retcode == 0) 
						{
							std::cout << std::endl << "SUCCESS." << std::endl << std::endl;

							if (!isTestServer) 
							{
								writeLastDevice(deviceNumber);
							}
						}

						else 
						{
							std::cout << std::endl << "FAILED." << std::endl << std::endl;
						}

						break;
					}
					else if (yesOrNo == 'n' || yesOrNo == 'N') {
						std::cout << "Please try again." << std::endl;
						break;
					}
					else {
						std::cout << "Please enter a valid selection ." << std::endl;
					}
				}
			}
			break;
		}

		// Add an employee to the database
		case '4':
		{
			char yesOrNo = 'l';
			while (yesOrNo != 'y' && yesOrNo != 'Y')
			{
				//reset yesOrNo so that the loop will come back around
				yesOrNo = 0;

				//Data entry block
				std::cout << "\nPlease enter the name of the employee (First 'Preffered' Last)." << std::endl;
				std::cin.ignore(80, '\n');
				std::wstring employeeName;
				std::getline(std::wcin, employeeName);

				std::cout << "\nPlease enter the employee's title." << std::endl;
				std::wstring employeeTitle;
				std::getline(std::wcin, employeeTitle);

				bool validInput = false;
				std::wstring employeeLocation;
				std::wstring employeeLocationID;

				while (!validInput) {

					std::cout << "\nPlease enter the location name for this employee." << std::endl;
					std::getline(std::wcin, employeeLocation);
					
					
					employeeLocationID = getLocationIDFromName(hStmt, employeeLocation);

					if (employeeLocationID == L"") {

						std::wcout << std::endl << L"There is no location with the name: " << employeeLocation << std::endl << L"Try again." << std::endl;

					}

					else {
						validInput = true;
					}
				}

				std::cout << "\nPlease enter the email address of the employee." << std::endl;
				std::wstring employeeEmailAddress;
				std::getline(std::wcin, employeeEmailAddress);

				std::cout << "\nPlease enter the phone number of the employee." << std::endl;
				std::wstring employeePhoneNumber;
				std::getline(std::wcin, employeePhoneNumber);

				std::cout << "\nPlease enter the extension of the employee." << std::endl;
				std::wstring employeeExtension;
				std::getline(std::wcin, employeeExtension);


				//Data validation block
				std::cout << "\nAre these the correct inputs? \n" << std::endl;

				std::cout << "Name: ";
				std::wcout << employeeName << std::endl;

				std::cout << "Title: ";
				std::wcout << employeeTitle << std::endl;

				std::cout << "Location: ";
				std::wcout << employeeLocation << std::endl;

				std::cout << "Email: ";
				std::wcout << employeeEmailAddress << std::endl;

				std::cout << "Phone number: ";
				std::wcout << employeePhoneNumber << std::endl;

				std::cout << "Phone extension: ";
				std::wcout << employeeExtension << std::endl;


				//Confirm before proceeding
				while (yesOrNo != 'y' && yesOrNo != 'Y' && yesOrNo != 'n' && yesOrNo != 'N')
				{
					std::cout << "(Y/N)" << std::endl;

					std::cin >> yesOrNo;
					if (yesOrNo == 'y' || yesOrNo == 'Y') {

						int retcode = addEmployee(hStmt, employeeName, employeeTitle, employeeLocationID, employeeEmailAddress, employeePhoneNumber, employeeExtension);

						if (retcode == 0) 
						{
							std::cout << std::endl << "SUCCESS." << std::endl << std::endl;
						}

						else
						{
							std::cout << std::endl << "FAILED." << std::endl << std::endl;
						}

					}
					else if (yesOrNo == 'n' || yesOrNo == 'N') {
						std::cout << "Please try again." << std::endl;
					}
					else {
						std::cout << "Please enter a valid selection ." << std::endl;
					}

				}
				break;
			}
			break;
		}

		case '5':
			// Remove an employee from the database
		{
			while (true)
			{
				std::cout << "\nPlease enter the email address of the employee you would like to remove from the database. Enter 'q' or 'Q' to stop" << std::endl;
				std::wstring employeeEmailAddress;
				std::wcin >> employeeEmailAddress;

				//clear error flags and stream buffer
				std::wcin.clear();
				std::wcin.ignore(10000, '\n');

				//Break if necessary
				if (employeeEmailAddress == L"q" || employeeEmailAddress == L"Q") {
					break;
				}
				// First, get the Employee ID
				std::wstring employeeID = getIdFromEmail(hStmt, employeeEmailAddress);

				// Bind parameter
				SQLLEN strlen = SQL_NTS;
				SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeID.c_str(), 0, &strlen);

				// Check that this person exists
				std::wstring checkEmployee = L"SELECT TOP (1) [Employee_Email], [Employee_Name], [Employee_Title], [Location_ID] FROM [Employees] WHERE Employee_ID = ?";

				SQLRETURN execResult1 = SQLExecDirect(hStmt, (SQLWCHAR*)checkEmployee.c_str(), SQL_NTS);
				std::wstring getEmail = getResult(hStmt, 1);

				SQLRETURN execResult2 = SQLExecDirect(hStmt, (SQLWCHAR*)checkEmployee.c_str(), SQL_NTS);
				std::wstring getName = getResult(hStmt, 2);

				SQLRETURN execResult3 = SQLExecDirect(hStmt, (SQLWCHAR*)checkEmployee.c_str(), SQL_NTS);
				std::wstring getTitle = getResult(hStmt, 3);

				SQLRETURN execResult4 = SQLExecDirect(hStmt, (SQLWCHAR*)checkEmployee.c_str(), SQL_NTS);
				std::wstring locationID = getResult(hStmt, 4);
				std::wstring getLocation = getLocationFromID(hStmt, locationID);

				if (getEmail == L"") {
					std::wcout << L"\nError. There is no employee with the email address: '" << employeeEmailAddress << "'" << std::endl;
				}

				else
				{
					//Confrim with user
					wchar_t confirmChoice = '0';
					std::wcout << std::endl << L"You are about to delete:" << std::endl;
					std::wcout << getName << std::endl;
					std::wcout << getTitle << std::endl;
					std::wcout << getLocation << std::endl;

					std::wcout << std::endl << L"Are you sure you want to remove this employee? (Y/N)" << std::endl;

					std::wcin >> confirmChoice;

					if (confirmChoice == L'Y' or confirmChoice == L'y') {
						int retcode = removeEmployee(hStmt, employeeID);

						if (retcode == 0)
						{
							std::cout << std::endl << "SUCCESS." << std::endl << std::endl;
						}

						else
						{
							std::cout << std::endl << "FAILED." << std::endl << std::endl;
						}

						break;
					}
				}
			}
			break;
		}

		// Remove a device from the database
		case '6':
		{
			while (true) {
				std::cout << "\nPlease scan the barcode on the device you would like to remove or enter its name using the keyboard. Enter 'q' or 'Q' to stop." << std::endl;
				std::wstring deviceNumber;
				std::wcin >> deviceNumber;

				//clear error flags and stream buffer
				std::wcin.clear();
				std::wcin.ignore(10000, '\n');

				if (deviceNumber == L"q" || deviceNumber == L"Q") {
					break;
				}

				bool isValid = checkValid(hStmt, L"[Devices]", L"[Device_Name]", L"Device_Name", deviceNumber);

				if (!isValid)
				{
					std::wcout << L"Unable to find device with the name: " << deviceNumber << L"." << std::endl << std::endl;
					break;
				}

				//Confirm delete

				std::wstring confirmChoice = L"0";
				std::wcout << std::endl << L"Are you sure you want to remove this device? (Y/N)" << std::endl;

				std::wcin >> confirmChoice;

				if (confirmChoice == L"Y" or confirmChoice == L"y") {
					int retcode = removeDevice(hStmt, deviceNumber);
					if (retcode == 0) 
					{
						std::cout << std::endl << "SUCCESS." << std::endl << std::endl;
					}
					else 
					{
						std::cout << std::endl << "FAILED." << std::endl << std::endl;
					}
					
					break;
				}
			}
			break;
		}

		// Prints the last entered device
		case '0':
		{
			readLastDevice();
			enterKey();
			break;
		}

		case 'q':
		{
			queryFinished = true;
			break;
		}
		case 'Q':
		{
			queryFinished = true;
			break;
		}

		case 't':
		{
			std::vector<std::wstring> resultsColumn;
			
			resultsColumn = getAllResults(hStmt, L"[Device_Model_Type]", L"[Device_Models]", L"DISTINCT ");
			 
			break;
		}

		default:
		{
			//if invalid input is entered, print message and loop
			std::cout << std::endl << "Please select a valid option." << std::endl << std::endl;
			break;
		}


		}
	}
return 0;
}
