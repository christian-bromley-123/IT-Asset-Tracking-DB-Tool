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

#include "main.hpp"

int main() {
	/*
	Main function used for entering the program.
	*/

	//Run the connect database function.
	int connectResult = connectDatabase(hEnv, hDbc, hStmt);

	// End the program if the connection fails
	if (connectResult != 0) {
		return -1;
	}


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
		//std::cin.ignore(10000, '\n');

		// User inputs selection
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

void assignDevice(SQLHSTMT hStmt) {
	/*
	Assigns an existing device to an employee. The device name and employee email address is provided by the user.
	*/


	while (true) {

		//Get the deviceNumber and empoyeeEmailAddress from the user
		std::cout << std::endl << "Please scan the device barcode on the device you would like to assign or enter it manually. Enter 'q' or 'Q' to stop." << std::endl;
		std::wcin >> deviceNumber;

		//clear error flags and stream buffer
		std::wcin.clear();
		std::wcin.ignore(10000, '\n');

		if (deviceNumber == L"q" || deviceNumber == L"Q") {
			break;
		}

		// Bind parameter
		SQLLEN strlen = SQL_NTS;
		SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check that this device exists
		std::wstring checkDevice = L"SELECT TOP (1) [Device_Name] FROM [Devices] WHERE Device_Name = ?";
		SQLRETURN execResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDevice.c_str(), SQL_NTS);

		std::wstring checkResult = getResult(hStmt, 1);

		if (checkResult == L"-1") {
			std::wcout << std::endl << L"Error. There is no device with the name: '" << deviceNumber << "'" << std::endl;
		}

		std::cout << std::endl << "Empoyee email address you would like to assign to device to. Enter 'q' or 'Q' to cancel." << std::endl;
		std::wcin >> employeeEmailAddress;
		
		//clear error flags and stream buffer
		std::wcin.clear();
		std::wcin.ignore(10000, '\n');

 		if (deviceNumber == L"q" || deviceNumber == L"Q") {
			break;
		}

		// Run a query to get EMPLOYEE ID from EMPLOYEE EMAIL

		employeeID = getIdFromEmail(hStmt, employeeEmailAddress);

		// Bind parameter
		strlen = SQL_NTS;
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeID.c_str(), 0, &strlen);

		// Check that this person exists
		std::wstring checkEmployee = L"SELECT TOP (1) [Employee_Email] FROM [Employees] WHERE Employee_ID = ?";
		execResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkEmployee.c_str(), SQL_NTS);
		checkResult = getResult(hStmt, 1);

		if (checkResult == L"-1") {
			std::wcout << L"\nError. There is no employee with the email address: '" << employeeEmailAddress << "'" << std::endl;
		}

		//Bind variables
		strlen = SQL_NTS;
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeID.c_str(), 0, &strlen);
		SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		//Generate Query
		std::wstring assignDeviceQuery = L"UPDATE [Devices] SET Currently_Issued_To = ? WHERE Device_Name = ?";

		//Execute
		SQLRETURN statementResult;
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)assignDeviceQuery.c_str(), SQL_NTS);

		//Free up the parameter for other functions
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
	}
	return;
}

void unassignDevice(SQLHSTMT hStmt) {

	/*
	Runs the query to unassign a device from a user, freeing it.
	*/

	while (true) {

		std::cout << "\nPlease scan the barcode on the device you would like to unassign or enter it using the keyboard. Enter 'q' or 'Q' to stop." << std::endl;
		std::wcin >> deviceNumber;


		if (deviceNumber == L"q" || deviceNumber == L"Q") {
			break;
		}

		// Bind parameter
		SQLLEN strlen = SQL_NTS;
		SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check that this device exists
		std::wstring checkDevice = L"SELECT TOP (1) [Device_Name] FROM [Devices] WHERE Device_Name = ?";
		SQLRETURN execResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDevice.c_str(), SQL_NTS);
		
		std::wstring checkResult = getResult(hStmt, 1);

		if (checkResult == L"-1") {
			std::wcout << L"\nError. There is no device with the name: '" << deviceNumber << "'" << std::endl;
		}


		//Binding deviceNumber to the first parameter
		strlen = SQL_NTS;

		SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);
		
		std::wstring unassignDevice = L"UPDATE [Devices] SET Currently_Issued_To = NULL WHERE Device_Name = ?";

		//Prepare and execute the query
		SQLRETURN prepareResult = SQLPrepare(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

		SQLRETURN statementResult = SQLExecute(hStmt);

		//Free up the parameter for other functions
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

		
	}
	return;
}

void newDevice(SQLHSTMT hStmt) {
	/*
	Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
	it will be issued.
	*/


	//Run this loop until the user is satisfied.
	while (yesOrNo != 'y' && yesOrNo != 'Y') {
		//reset yesOrNo so that the loop will come back around
		yesOrNo = 0;

		//Gathering inputs, Device_Name, Serial_Tag, Device_Model_ID, Currently_Issued_To, In_Service_Date
		std::cout << "\nPlease scan the computer's barcode or enter the computer name." << std::endl;
		std::cin.ignore(80, '\n');
		std::getline(std::wcin, deviceNumber);

		std::cout << "\nPlease scan or enter the computer's serial tag." << std::endl;
		std::getline(std::wcin, serialTag);

		std::cout << "\nPlease scan or enter the computer's Device Model ID." << std::endl;
		std::getline(std::wcin, deviceModelId);
		
		std::cout << "\nPlease enter the email address of the employee who is being issued this computer." << std::endl;

		std::getline(std::wcin, employeeEmailAddress);
		currentlyIssuedTo = getIdFromEmail(hStmt, employeeEmailAddress);
		std::cout << std::endl;

		std::cout << "\nPlease enter the date in the following format: M/D/YYYY" << std::endl;
		std::getline(std::wcin, inServiceDate);
		std::cout << std::endl;

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

		std::cout << "Issued to: ";
		std::wcout << employeeEmailAddress << std::endl;

		std::cout << "Date: ";
		std::wcout << inServiceDate << std::endl;

		//Confirm before proceeding
		while (yesOrNo != 'y' && yesOrNo != 'Y' && yesOrNo != 'n' && yesOrNo != 'N') {
			std::cout << "(Y/N)" << std::endl;

			std::cin >> yesOrNo;
			if (yesOrNo == 'y' || yesOrNo == 'Y') {

				//Bind parameters
				SQLLEN strlen = SQL_NTS;
				SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);
				SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialTag.c_str(), 0, &strlen);
				SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
				SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)currentlyIssuedTo.c_str(), 0, &strlen);
				SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)inServiceDate.c_str(), 0, &strlen);

				//Generate query
				std::wstring addDeviceQuery = L"INSERT INTO Devices (Device_Name, Serial_Tag, Device_Model_ID, Currently_Issued_To, In_Service_Date) VALUES (?, ?, ?, ?, ?)";

				//Execute 
				SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

				// Free binds
				SQLFreeStmt(hStmt, SQL_RESET_PARAMS);


				//THis program tends to put a lot of space characters to the right
				bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);
				addDeviceQuery = L"UPDATE Devices SET Device_Name = RTRIM(Device_Name) WHERE Device_Name = ?";
				result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

				bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialTag.c_str(), 0, &strlen);
				addDeviceQuery = L"UPDATE Devices SET Serial_Tag = RTRIM(Serial_Tag) WHERE Serial_Tag = ?";
				result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

				bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
				addDeviceQuery = L"UPDATE Devices SET Device_Model_ID = RTRIM(Device_Model_ID) WHERE Device_Model_ID = ?";
				result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

				bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)inServiceDate.c_str(), 0, &strlen);
				addDeviceQuery = L"UPDATE Devices SET In_Service_Date = RTRIM(In_Service_Date) WHERE In_Service_Date = ?";
				result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

				//Free parameters
				SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

				writeLastDevice(deviceNumber);

			}
			else if (yesOrNo == 'n' || yesOrNo == 'N') {
				std::cout << "Please try again." << std::endl;
			}
			else {
				std::cout << "Please enter a valid selection ." << std::endl;
			}
		}
		yesOrNo = 0;
		return;
	}
	//Free up the parameter for other functions
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	return;
}

void addEmployee(SQLHSTMT hStmt) {
	/*
	Add an employee to the database.
	*/

	while (yesOrNo != 'y' && yesOrNo != 'Y') {
		//reset yesOrNo so that the loop will come back around
		yesOrNo = 0;

		//Data entry block
		std::cout << "\nPlease enter the name of the employee (First 'Preffered' Last)." << std::endl;
		std::cin.ignore(80, '\n'); 
		std::getline(std::wcin, employeeName);

		std::cout << "\nPlease enter the employee's title." << std::endl;
		
		std::getline(std::wcin, employeeTitle);

		std::cout << "\nPlease enter the Location ID number for this employee." << std::endl;
		
		std::getline(std::wcin, employeeLocation);

		std::cout << "\nPlease enter the email address of the employee." << std::endl;
		
		std::getline(std::wcin, employeeEmailAddress);

		std::cout << "\nPlease enter the phone number of the employee." << std::endl;
		
		std::getline(std::wcin, employeePhoneNumber);

		std::cout << "\nPlease enter the extension of the employee." << std::endl;
		
		std::getline(std::wcin, employeeExtension);


		
		//Data validation block
		std::cout << "\nAre these the correct inputs? \n" << std::endl;

		std::cout << "Name: ";
		std::wcout << employeeName << std::endl;

		std::cout << "Title: ";
		std::wcout << employeeTitle << std::endl;

		std::cout << "Location ID: ";
		std::wcout << employeeLocation << std::endl;

		std::cout << "Email: ";
		std::wcout << employeeEmailAddress << std::endl;

		std::cout << "Phone number: ";
		std::wcout << employeePhoneNumber << std::endl;

		std::cout << "Phone extension: ";
		std::wcout << employeeExtension << std::endl;
		

		//Confirm before proceeding
		while (yesOrNo != 'y' && yesOrNo != 'Y' && yesOrNo != 'n' && yesOrNo != 'N') {
			std::cout << "(Y/N)" << std::endl;

			std::cin >> yesOrNo;
			if (yesOrNo == 'y' || yesOrNo == 'Y') {

				//Bind parameters
				SQLLEN strlen = SQL_NTS;
				SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeName.c_str(), 0, &strlen);
				SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeTitle.c_str(), 0, &strlen);
				SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeLocation.c_str(), 0, &strlen);
				SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeEmailAddress.c_str(), 0, &strlen);
				SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeePhoneNumber.c_str(), 0, &strlen);
				SQLRETURN bindResult6 = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeExtension.c_str(), 0, &strlen);

				//Exectution string
				std::wstring newEmployeeString = L"INSERT INTO Employees(Employee_Name, Employee_Title, Location_ID, Employee_Email, Employee_Phone_Number, Employee_Ext) VALUES (?, ?, ?, ?, ?, ?)";

				//Execute
				SQLExecDirect(hStmt, (SQLWCHAR*)newEmployeeString.c_str(), SQL_NTS);

				SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

			}
			else if (yesOrNo == 'n' || yesOrNo == 'N') {
				std::cout << "Please try again." << std::endl;
			}
			else {
				std::cout << "Please enter a valid selection ." << std::endl;
			}

		}
	}
	return;
}

void removeEmployee(SQLHSTMT hStmt) {
	/*
	Remove an employee from the database.

	PAUSE ON ERROR
	*/

	while (true) {

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
		employeeID = getIdFromEmail(hStmt, employeeEmailAddress);

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
		
		if (getEmail == L"-1") {
			std::wcout << L"\nError. There is no employee with the email address: '" << employeeEmailAddress << "'" << std::endl;
		}
		
		else {
			//Confrim with user
			wchar_t confirmChoice = '0';
			while (true) {
				std::wcout << std::endl << L"You are about to delete:" << std::endl;
				std::wcout <<  getName << std::endl;
				std::wcout <<  getTitle << std::endl;
				std::wcout << getLocation << std::endl;

				std::wcout << std::endl << L"Are you sure you want to remove this employee? (Y/N)" << std::endl;

				std::wcin >> confirmChoice;

				if (confirmChoice != L'Y' and confirmChoice != L'y') {
					return;
				}
				break;
			}

			// Unassign device
			std::wstring unassignAllDevices = L"UPDATE [Devices] SET Currently_Issued_to = NULL WHERE Currently_Issued_to = ?";
			SQLRETURN statementResult;
			statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

			// Finally, remove the employee
			std::wstring removeEmpQuery = L"DELETE FROM [Employees] WHERE Employee_ID = ?";
			statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)removeEmpQuery.c_str(), SQL_NTS);

			// Unbind Parameter
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		}
		
	}
	return;
}

void removeDevice(SQLHSTMT hStmt) {
	/*
	Remove a device from the database. 
	*/

	while (true) {

		std::cout << "\nPlease scan the barcode on the device you would like to remove or enter its name using the keyboard. Enter 'q' or 'Q' to stop." << std::endl;
		std::wcin >> deviceNumber;

		//clear error flags and stream buffer
		std::wcin.clear();
		std::wcin.ignore(10000, '\n');

		if (deviceNumber == L"q" || deviceNumber == L"Q") {
			break;
		}


		//Binding deviceNumber to the first parameter
		SQLLEN strlen = SQL_NTS;
		SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);


		//Make sure the device is actually there
		std::wstring findDevice = L"SELECT TOP (1) [Device_Name] FROM [Devices] WHERE Device_Name = ?";
		SQLRETURN selectResult = SQLExecDirect(hStmt, (SQLWCHAR*)findDevice.c_str(), SQL_NTS);

		std::wstring checkResult = getResult(hStmt, 1);
		
		if (checkResult == L"-1") {

			std::wcout << L"\nError. There is no device by the name: '" << deviceNumber << "'" << std::endl;

		}

		else {
			//Confrim with user
			wchar_t confirmChoice = '0';
			while (true) {

				std::wcout << std::endl << L"Are you sure you want to remove this device? (Y/N)" << std::endl;

				std::wcin >> confirmChoice;

				if (confirmChoice != L'Y' and confirmChoice != L'y') {
					return;
				}
				break;
			}

			//Delete the device
			std::wstring unassignDevice = L"DELETE FROM [Devices] WHERE Device_Name = ?";
			SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

		}

		//Free up the parameter for other functions
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		
	}
	return;
}

void writeLastDevice(std::wstring deviceNumber) {

	//write the last device to the last device file
	std::wofstream lastDevice("LastDevice.txt");
	lastDevice << deviceNumber;
	lastDevice.close();

	return;
}

void readLastDevice() {

	//Get the last device
	std::wifstream lastDevice("LastDevice.txt");
	lastDevice >> deviceNumber;
	lastDevice.close();

	std::wcout << std::endl << L"Last device entered: " + deviceNumber << std::endl << std::endl;

	return;
}

int connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt) {
	/*
	
	Takes handles and the connection string to connect to a database.

	*/

	//Allocate an environment

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
	{
		fwprintf(stderr, L"Unable to allocate an environment handle\n");
		exit(-1);
	} 

	//Register
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

	//Allocate a connection

	SQLRETURN dbcHandleReturn = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	SQLRETURN result;

	//Get Connection string from config
	std::cout << "ENTER '1' FOR TEST SERVER. ENTER '2' FOR LIVE SERVER. FOR COPYRIGHT AND LICENSE INFORMATION ENTER 'C'." << std::endl;
	std::cin >> dbChoice;

	if (dbChoice == 'C') {
		std::cout << "This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU Affero General Public License for more details. You should have received a copy of the GNU Affero General Public License along with this program.If not, see <https://www.gnu.org/licenses/>." << std::endl << std::endl;

		std::cout << "ENTER '1' FOR TEST SERVER. ENTER '2' FOR LIVE SERVER." << std::endl;

		std::cin >> dbChoice; //Fall through
	}

	std::wstring connectionString = getConString(dbChoice);

	result = SQLDriverConnect(hDbc, NULL, (SQLWCHAR*)connectionString.c_str(), SQL_NTS, NULL, 1, NULL, SQL_DRIVER_COMPLETE);

	if (result == SQL_SUCCESS) {
		fwprintf(stderr, L"Connection Established.\n\n\n");


	}
	else if (result == SQL_SUCCESS_WITH_INFO) {
		fwprintf(stderr, L"Connection Established.\n\n\n");

	}
	else {
		fwprintf(stderr, L"Connection failed.\n\n\n");
		SQLRETURN diagResult = diagSQLError(SQL_HANDLE_DBC, hDbc);
		enterKey();
		return -1;
	}

	SQLRETURN stmtHandleReturn = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	return 0;
}

std::wstring getConString(char dbChoice) {
	/*
	Gets the connection string from a config file.
	*/

	//Create an array of two elements
	std::wstring line[] = { L"", L"" };
	std::wfstream file;

	//Open the config file and populate the first two elements of the array

	file.open("config.cfg", std::ios::in);

	if (file.is_open()) {

		for (int i = 0; i < 2; i++) {
			
			std::getline(file, line[i]);

		}
	}

	else {
		std::cout << "Error opening file. Check if the config file is present.";
		return L"";
	}

	//Select the appropriate line or return an error code
	if (dbChoice == '1') {
		return line[0];
	}
	else if(dbChoice == '2') {
		return line[1];
		}
	else {
		return L"-1";
	}
}

SQLRETURN diagSQLError(int sqlHandle, SQLHANDLE handle) {
	/*
	
	Sets up SQLGetDiagRec() and prints debug mesages to the terminal.

	*/

	SQLWCHAR SQLState[6];
	SQLWCHAR* SQLStatePtr;

	SQLWCHAR SQLMessage[128];
	SQLWCHAR* SQLMessagePtr;

	SQLSMALLINT textLength = 0;
	SQLSMALLINT* textLengthPtr = &textLength;


	SQLStatePtr = SQLState;
	SQLMessagePtr = SQLMessage;


	SQLRETURN diagResult = SQLGetDiagRec(sqlHandle, handle, 1, SQLStatePtr, NULL, SQLMessagePtr, 128, textLengthPtr);

	std::wstring errorCode = SQLStatePtr;
	std::wstring errorMessage(SQLMessagePtr, *textLengthPtr);

	std::wcout << std::endl << errorMessage << L"\nError code : \n" << errorCode << std::endl;

	enterKey();

	return diagResult;
}

std::wstring getIdFromEmail(SQLHANDLE handle, std::wstring employeeEmail) {
	/*
	
		Called when you want to get the ID number associated with an email address. Email addresses are much easier to "guess"
		than ID numbers, so when a query expects an ID number, you can feed it this function instead.

	*/
	
	// Bind email variable to param 1
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeEmail.c_str(), 0, &strlen);

	std::wstring getIdQuery = L"SELECT TOP (1) [Employee_ID] FROM Employees WHERE Employee_Email = ?";

	// Execute the query
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(handle, (SQLWCHAR*)getIdQuery.c_str(), SQL_NTS);

	if (statementResult != 0 and statementResult != 1) {
		diagSQLError(SQL_HANDLE_STMT, hStmt);
	}
	std::wstring idString = getResult(hStmt, 1);

	return idString;
}

std::wstring getLocationFromID(SQLHANDLE handle, std::wstring locationID) {
	// Bind Location ID to param 1
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)locationID.c_str(), 0, &strlen);

	std::wstring getIdQuery = L"SELECT TOP (1) [Location_Name] FROM Locations WHERE Location_ID = ?";

	// Execute the query
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(handle, (SQLWCHAR*)getIdQuery.c_str(), SQL_NTS);

	if (statementResult != 0 and statementResult != 1) {
		diagSQLError(SQL_HANDLE_STMT, hStmt);
	}

	std::wstring locationName = getResult(hStmt, 1);

	return locationName;
}

std::wstring getResult(SQLHANDLE handle, int column) {
	/*
	Return the result from a select query
	*/

	// Set the cursor to the first row of results
	SQLRETURN fetchReturn = SQLFetch(handle);

	// Set up result variables 
	SQLRETURN getResult;
	SQLRETURN getResultError;
	std::wstring getResultString;

	//Set string buffer
	getResultString.resize(64);
	SQLLEN stringLength = 64;
	SQLLEN* stringLengthPtr = &stringLength;

	// Get the data in the column passed to the function
	getResult = SQLGetData(handle, column, SQL_C_WCHAR, (void*)getResultString.data(), getResultString.size(), stringLengthPtr);

	// Return an error
	if (getResult == -1) {
		return L"-1";
	}

	//Handle errors
	if (getResult != 0 and getResult != 1) {
		getResultError = diagSQLError(SQL_HANDLE_STMT, handle);
	}

	//Close cursor so that more queries can run
	SQLCloseCursor(handle);

	return getResultString;
}

void enterKey() {
	char key;
	std::cout << "Enter any character to continue" << std::endl;
	while (true) {
		std::cin >> key;
		std::cout << std::endl;
		break;
	}
	return;
}
