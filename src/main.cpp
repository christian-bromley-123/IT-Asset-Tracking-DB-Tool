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
#include "..\include\tests.hpp"
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
				std::wstring deviceTable;
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
					deviceTable = getDeviceTableFromModelId(hStmt, getModelIdFromDeviceName(hStmt, deviceNumber));

					bool isValid = false;

					// Check if the device name is in the system
					if (deviceTable == L"Computers")
					{
						isValid = checkValid(hStmt, L"[Computers]", L"[Computer_Name]", L"Computer_Name", deviceNumber);
					}

					else if (deviceTable == L"Peripherals")
					{
						isValid = checkValid(hStmt, L"[Peripherals]", L"[Peripherals_Name]", L"Peripherals_Name", deviceNumber);
					}

					else if (deviceTable == L"Hotspots")
					{
						isValid = checkValid(hStmt, L"[Hotspots]", L"[Phone_Number]", L"Phone_Number", deviceNumber);
					}

					else
					{
						isValid = checkValid(hStmt, L"[Office_Equipment]", L"[Equipment_Name]", L"Equipment_Name", deviceNumber);
					}

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
				if (deviceTable == L"Computers" || deviceTable == L"Peripherals")
				{
					while (firstUser != L"1" && firstUser != L"0")
					{


						std::cout << std::endl << "Is this device new? (Y/N)" << std::endl;
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
				}
				
				int retcode;
				if (deviceTable == L"Computers")
				{
					retcode = assignComputer(hStmt, deviceNumber, employeeEmailAddress, issueDate, firstUser);
				}

				else if (deviceTable == L"Peripherals")
				{
					retcode = assignPeripheral(hStmt, deviceNumber, employeeEmailAddress, issueDate, firstUser);
				}

				else if (deviceTable == L"Hotspots")
				{
					retcode = assignHotspot(hStmt, deviceNumber, employeeEmailAddress, issueDate);
				}

				else
				{
					retcode = assignEquipment(hStmt, deviceNumber, employeeEmailAddress, issueDate);
				}

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

				std::wstring deviceTable;
				deviceTable = getDeviceTableFromModelId(hStmt, getModelIdFromDeviceName(hStmt, deviceNumber));

				bool isValid = false;

				if (deviceTable == L"Computers")
				{
					isValid = checkValid(hStmt, L"[Computers]", L"[Computer_Name]", L"Computer_Name", deviceNumber);
				}

				else if (deviceTable == L"Peripherals")
				{
					isValid = checkValid(hStmt, L"[Peripherals]", L"[Peripherals_Name]", L"Peripherals_Name", deviceNumber);
				}

				else if (deviceTable == L"Hotspots")
				{
					isValid = checkValid(hStmt, L"[Hotspots]", L"[Phone_Number]", L"Phone_Number", deviceNumber);
				}

				else 
				{
					isValid = checkValid(hStmt, L"[Office_Equipment]", L"[Equipment_Name]", L"Equipment_Name", deviceNumber);
				}

				if (isValid)
				{	
					int retcode;
					if (deviceTable == L"Computers")
					{
						retcode = unassignComputer(hStmt, deviceNumber);
					}

					else if (deviceTable == L"Peripherals")
					{
						retcode = unassignPeripheral(hStmt, deviceNumber);
					}

					else if (deviceTable == L"Hotspots")
					{
						retcode = unassignHotspot(hStmt, deviceNumber);
					}

					else
					{
						retcode = unassignEquipment(hStmt, deviceNumber);
					}

					if (retcode == 0) {
						std::cout << std::endl << "SUCCESS." << std::endl << std::endl;
					}
					else {
						std::cout << std::endl << "FAILED." << std::endl << std::endl;
					}
					break;
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
			// Get Device Model
			std::wstring deviceModel;
			
			bool modelFound = false;
			std::cin.ignore(80, '\n');
			while (!modelFound)
			{
				std::wcout << std::endl << L"Please enter the model name of the device you are entering. Press 'h' for help finding the name." << std::endl;
				std::getline(std::wcin, deviceModel);

				if (deviceModel == L"h" || deviceModel == L"H")
				{
					// Help Loop
					bool typeFound = false;
					while (!typeFound)
					{
						std::wcout << std::endl << L"Please enter the device type. Press 'h' for a list of types." << std::endl;
						std::wstring deviceType;
						std::getline(std::wcin, deviceType);

						if (deviceType == L"h" || deviceType == L"H")
						{
							std::vector<std::wstring> deviceTypeList = getColumn(hStmt, 1, L"[Device_Models]", L"[Device_Model_Type]");

							// Print list of types to the user
							std::wcout << std::endl << L"Device Types:" << std::endl;

							for (int i = 0; i < deviceTypeList.size(); i++)
							{
								std::wcout << deviceTypeList[i] << std::endl;
							}
						}

						else
						{
							// Check that the model type is valid
							if (checkValid(hStmt, L"[Device_Models]", L"[Device_Model_Type]", L"Device_Model_Type", deviceType))
							{
								typeFound = true;

								std::vector<std::wstring> deviceModelList;
								deviceModelList = getColumn(hStmt, 1, L"[Device_Models]", L"[Device_Model_Name]", L"Device_Model_Type", deviceType);

								// Print list of types to the user

								std::wcout << std::endl << L"Device Types:" << std::endl;

								for (int i = 0; i < deviceModelList.size(); i++)
								{
									std::wcout << deviceModelList[i] << std::endl;
								}
							}

							else
							{
								std::wcout << std::endl << L"Invalid device type." << std::endl;
							}
						}
					}
				}

				else
				{
					if (checkValid(hStmt, L"[Device_Models]", L"[Device_Model_Name]", L"Device_Model_Name", deviceModel))
					{
						modelFound = true;
					}

					else
					{
						std::wcout << std::endl << L"Invalid device model name." << std::endl;
					}
				}
			}
			
			// Find Type from model ID
			std::vector<std::wstring> deviceModelTableList = getColumn(hStmt, 1, L"[Device_Models]", L"[Device_Model_Table]", L"Device_Model_Name", deviceModel);
			std::wstring deviceModelTable = deviceModelTableList[0];

			

			if (deviceModelTable == L"Computers")
			{
				bool computerQueryFinished = false;

				std::wstring computerName;
				std::wstring serialNumber;
				std::wstring datePurchased;
				std::wstring cost;
				std::wstring operatingSystem;

				while (!computerQueryFinished)
				{
					// Needs Computer_Name, Serial_Number, deviceModel (already given), Date_Purchased, Cost, Operating_System
					std::wcout << std::endl << L"Please enter the computer's name:" << std::endl;
					getline(std::wcin, computerName);
					
					std::wcout << std::endl << L"Please enter the computer's serial number:" << std::endl;
					getline(std::wcin, serialNumber);
					
					std::wcout << std::endl << L"Please enter the date that the computer was purchased:" << std::endl;
					getline(std::wcin, datePurchased);

					std::wcout << std::endl << L"Please enter the price that the computer was purchased for:" << std::endl;
					getline(std::wcin, cost);

					std::wcout << std::endl << L"Please enter the operating system of the computer:" << std::endl;
					getline(std::wcin, operatingSystem);

					std::wcout << std::endl << L"Are these the correct inputs?" << std::endl;

					std::wcout << L"Computer Name: " << computerName << std::endl;
					std::transform(serialNumber.begin(), serialNumber.end(), serialNumber.begin(), ::toupper);
					std::wcout << L"Serial Number: " << serialNumber << std::endl;
					std::wcout << L"Computer Model: " << deviceModel << std::endl;
					std::wcout << L"Date purchased: " << datePurchased << std::endl;
					std::wcout << L"Price: " << cost << std::endl;
					std::wcout << L"Operating System: " << operatingSystem << std::endl;
					std::wcout << L"(Y/N)" << std::endl;

					std::wstring confirmedInput;
					getline(std::wcin, confirmedInput);

					if (confirmedInput == L"Y" || confirmedInput == L"y")
					{
						computerQueryFinished = true;
					}

					else
					{
						std::wcout << std::endl << L"Please try again." << std::endl;
					}
				}

				//Run query and make sure it worked.
				std::wstring deviceModelId = getModelIdFromModelName(hStmt, deviceModel);
				int newComputerRetCode = newComputer(hStmt, computerName, serialNumber, deviceModelId, datePurchased, cost, operatingSystem, isTestServer);

				if (newComputerRetCode == 0)
				{
					std::wcout << std::endl << L"SUCCESS." << std::endl;
				}

				else
				{
					std::wcout << L"FAILED.";
				}

			}
			
			else if (deviceModelTable == L"Peripherals")
			{
				bool peripheralQueryFinished = false;

				std::wstring peripheralName;
				std::wstring serialNumber;
				std::wstring datePurchased;
				std::wstring cost;
				
				while (!peripheralQueryFinished)
				{
					// Needs Computer_Name, Serial_Number, deviceModel (already given), Date_Purchased, Cost, Operating_System
					std::wcout << std::endl << L"Please enter the devices' name:" << std::endl;
					getline(std::wcin, peripheralName);

					std::wcout << std::endl << L"Please enter the devices' serial number:" << std::endl;
					getline(std::wcin, serialNumber);

					std::wcout << std::endl << L"Please enter the date that the device was purchased:" << std::endl;
					getline(std::wcin, datePurchased);

					std::wcout << std::endl << L"Please enter the price that the device was purchased for:" << std::endl;
					getline(std::wcin, cost);

					std::wcout << std::endl << L"Are these the correct inputs?" << std::endl;

					std::wcout << L"Device Name: " << peripheralName << std::endl;
					std::transform(serialNumber.begin(), serialNumber.end(), serialNumber.begin(), ::toupper);
					std::wcout << L"Serial Number: " << serialNumber << std::endl;
					std::wcout << L"Device Model: " << deviceModel << std::endl;
					std::wcout << L"Date purchased: " << datePurchased << std::endl;
					std::wcout << L"Price: " << cost << std::endl;
					std::wcout << L"(Y/N)" << std::endl;

					std::wstring confirmedInput;
					getline(std::wcin, confirmedInput);

					if (confirmedInput == L"Y" || confirmedInput == L"y")
					{
						peripheralQueryFinished = true;
					}

					else
					{
						std::wcout << std::endl << L"Please try again." << std::endl;
					}
				}

				//Run query and make sure it worked.
				std::wstring deviceModelId = getModelIdFromModelName(hStmt, deviceModel);
				int newComputerRetCode = newPeripheral(hStmt, peripheralName, serialNumber, deviceModelId, datePurchased, cost, isTestServer);

				if (newComputerRetCode == 0)
				{
					std::wcout << std::endl << L"SUCCESS." << std::endl;
				}

				else
				{
					std::wcout << std::endl << L"FAILED." << std::endl;
				}
			}

			else if (deviceModelTable == L"Hotspots")
			{
				bool hotspotQueryFinished = false;

				std::wstring phoneNumber;
				std::wstring imeiNumber;
				std::wstring datePurchased;
				std::wstring cost;

				while (!hotspotQueryFinished)
				{
					// Needs Computer_Name, Serial_Number, deviceModel (already given), Date_Purchased, Cost, Operating_System
					std::wcout << std::endl << L"Please enter the hotspots' number:" << std::endl;
					getline(std::wcin, phoneNumber);

					std::wcout << std::endl << L"Please enter the hotspots' IMEI number:" << std::endl;
					getline(std::wcin, imeiNumber);

					std::wcout << std::endl << L"Please enter the date that the hotspot was purchased:" << std::endl;
					getline(std::wcin, datePurchased);

					std::wcout << std::endl << L"Please enter the price that the hotspot was purchased for:" << std::endl;
					getline(std::wcin, cost);

					std::wcout << std::endl << L"Are these the correct inputs?" << std::endl;

					std::wcout << L"Hotspot Number: " << phoneNumber << std::endl;
					std::transform(imeiNumber.begin(), imeiNumber.end(), imeiNumber.begin(), ::toupper);
					std::wcout << L"IMEI Number: " << imeiNumber << std::endl;
					std::wcout << L"Device Model: " << deviceModel << std::endl;
					std::wcout << L"Date purchased: " << datePurchased << std::endl;
					std::wcout << L"Price: " << cost << std::endl;
					std::wcout << L"(Y/N)" << std::endl;

					std::wstring confirmedInput;
					getline(std::wcin, confirmedInput);

					if (confirmedInput == L"Y" || confirmedInput == L"y")
					{
						hotspotQueryFinished = true;
					}

					else
					{
						std::wcout << std::endl << L"Please try again." << std::endl;
					}
				}

				//Run query and make sure it worked.
				std::wstring deviceModelId = getModelIdFromModelName(hStmt, deviceModel);
				int newComputerRetCode = newHotspot(hStmt, phoneNumber, imeiNumber, deviceModelId, datePurchased, cost, isTestServer);

				if (newComputerRetCode == 0)
				{
					std::wcout << std::endl << L"SUCCESS." << std::endl;
				}

				else
				{
					std::wcout << std::endl << L"FAILED." << std::endl;
				}
			}

			else if (deviceModelTable == L"Office_Equipment")
			{
				bool equipmentQueryFinished = false;

				std::wstring equipmentName;
				std::wstring serialNumber;
				std::wstring datePurchased;
				std::wstring cost;

				while (!equipmentQueryFinished)
				{
					// Needs Computer_Name, Serial_Number, deviceModel (already given), Date_Purchased, Cost, Operating_System
					std::wcout << std::endl << L"Please enter the equipment's number:" << std::endl;
					getline(std::wcin, equipmentName);

					std::wcout << std::endl << L"Please enter the equipment's serial number:" << std::endl;
					getline(std::wcin, serialNumber);

					std::wcout << std::endl << L"Please enter the date that the equipment was purchased:" << std::endl;
					getline(std::wcin, datePurchased);

					std::wcout << std::endl << L"Please enter the price that the eqipment was purchased for:" << std::endl;
					getline(std::wcin, cost);

					std::wcout << std::endl << L"Are these the correct inputs?" << std::endl;

					std::wcout << L"Equipment number: " << equipmentName << std::endl;
					std::transform(serialNumber.begin(), serialNumber.end(), serialNumber.begin(), ::toupper);
					std::wcout << L"Serial Number: " << serialNumber << std::endl;
					std::wcout << L"Device Model: " << deviceModel << std::endl;
					std::wcout << L"Date purchased: " << datePurchased << std::endl;
					std::wcout << L"Price: " << cost << std::endl;
					std::wcout << L"(Y/N)" << std::endl;

					std::wstring confirmedInput;
					getline(std::wcin, confirmedInput);

					if (confirmedInput == L"Y" || confirmedInput == L"y")
					{
						equipmentQueryFinished = true;
					}

					else
					{
						std::wcout << std::endl << L"Please try again." << std::endl;
					}
				}

				//Run query and make sure it worked.
				std::wstring deviceModelId = getModelIdFromModelName(hStmt, deviceModel);
				int newComputerRetCode = newEquipment(hStmt, equipmentName, serialNumber, deviceModelId, datePurchased, cost, isTestServer);

				if (newComputerRetCode == 0)
				{
					std::wcout << std::endl << L"SUCCESS." << std::endl;
				}

				else
				{
					std::wcout << std::endl << L"FAILED." << std::endl;
				}
			}

			else
			{
				std::wcout << std::endl << L"An error occured when selecting a table to place the new device into. Please review the 'Device_Models' table in the database." << std::endl;
				enterKey();
				return -1;
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

				// Check all 4 device tables, branch accordingly

				bool isInComputers = checkValid(hStmt, L"[Computers]", L"[Computer_Name]", L"Computer_Name", deviceNumber);
				bool isInPeripherals = checkValid(hStmt, L"[Peripherals]", L"[Peripheral_Name]", L"Peripheral_Name", deviceNumber);
				bool isInHotspots = checkValid(hStmt, L"[Hotspots]", L"[IMEI_Number]", L"IMEI_Number", deviceNumber);
				bool isInEquipment = checkValid(hStmt, L"[Office_Equipment]", L"[Equipment_Name]", L"Equipment_Name", deviceNumber);

				//Confirm delete

				std::wstring confirmChoice = L"0";
				std::wcout << std::endl << L"Are you sure you want to remove this device? (Y/N)" << std::endl;

				std::wcin >> confirmChoice;

				int retcode;

				if (confirmChoice == L"Y" or confirmChoice == L"y") {
					if (isInComputers)
					{
						retcode = removeComputer(hStmt, deviceNumber);
					}

					else if (isInPeripherals)
					{
						retcode = removePeripheral(hStmt, deviceNumber);
					}

					else if (isInHotspots)
					{
						retcode = removeHotspot(hStmt, deviceNumber);
					}

					else if (isInEquipment)
					{
						retcode = removeEquipment(hStmt, deviceNumber);
					}

					else
					{
						std::wcout << L"Unable to find device with the name: " << deviceNumber << L"." << std::endl << std::endl;
						break;
					}

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

		case '7':
		{
			int retcode = removeEmployee(hStmt, L"493");
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
			std::vector<std::wstring> retVector;
			retVector = testAll(hStmt, isTestServer);
			std::wcout << std::endl << L"Test results:";

			for (int i = 0; i<retVector.size();i++)
			{
				std::wcout << std::endl << retVector[i];
			}

			std::wcout << std::endl;
			enterKey();
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
