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

#include "..\include\DBFunctions.hpp"

int assignDevice(SQLHSTMT hStmt, std::wstring deviceNumber , std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser) {
	/*

		Assigns an existing device to an employee. The device name and employee email address is provided by the user.

	*/


	while (true) {
		// Get employee ID from email
		std::wstring employeeID = getIdFromEmail(hStmt, employeeEmailAddress);

		//Bind variables
		SQLLEN    strlen = SQL_NTS;
		SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeID.c_str(), 0, &strlen);
		SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)issueDate.c_str(), 0, &strlen);
		SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)firstUser.c_str(), 0, &strlen);
		SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		//Generate Query
		std::wstring assignDeviceQuery = L"UPDATE [Devices] SET Currently_Issued_To = ?, Date_Issued = ?, Issued_New = ? WHERE Device_Name = ?";

		//Execute
		SQLRETURN statementResult;
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)assignDeviceQuery.c_str(), SQL_NTS);

		//TRIM

		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		std::wstring trimDateQuery = L"UPDATE Devices SET Date_Issued = RTRIM(Date_Issued) WHERE Device_Name = ?";
		SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)trimDateQuery.c_str(), SQL_NTS);
		std::wstring trimNewQuery = L"UPDATE Devices SET Issued_New = RTRIM(Issued_New) WHERE Device_Name = ?";
		result = SQLExecDirect(hStmt, (wchar_t*)trimNewQuery.c_str(), SQL_NTS);


		//Rebind
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check query result UPDATE TO INCLUDE ALL NEW INFO
		std::wstring checkUserResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Devices] WHERE Device_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkUserResultQuery.c_str(), SQL_NTS);
		std::wstring checkUserResult = getResult(hStmt, 1);

		std::wstring checkDateResultQuery = L"SELECT TOP (1) [Date_Issued] FROM [Devices] WHERE Device_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDateResultQuery.c_str(), SQL_NTS);
		std::wstring checkDateResult = getResult(hStmt, 1);

		std::wstring checkNewResultQuery = L"SELECT TOP (1) [Issued_New] FROM [Devices] WHERE Device_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkNewResultQuery.c_str(), SQL_NTS);
		std::wstring checkNewResult = getResult(hStmt, 1);

		// Copy and reformat values to compare against
		std::wstring dateFromDB = issueDate;
		std::wstring newBoolFromDB = firstUser;

		dateFromDB.resize(64);
		newBoolFromDB.resize(64);

		if (checkUserResult == employeeID && checkDateResult == dateFromDB && checkNewResult == newBoolFromDB) {
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
			return 0;
		}

		else {
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
			return -1;
		}
	}
}

int unassignDevice(SQLHSTMT hStmt, std::wstring deviceNumber) {

	/*

		Runs the query to unassign a device from a user, freeing it.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;

	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	std::wstring unassignDevice = L"UPDATE [Devices] SET Currently_Issued_To = NULL WHERE Device_Name = ?";

	//Prepare and execute the query
	SQLRETURN prepareResult = SQLPrepare(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	SQLRETURN statementResult = SQLExecute(hStmt);

	//Rebind
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	// Check query result
	std::wstring checkExecResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Devices] WHERE Device_Name = ?";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkExecResultQuery.c_str(), SQL_NTS);

	std::wstring checkExecResult = getResult(hStmt, 1);

	std::wstring nullString = L"";
	nullString.resize(64);

	if (checkExecResult == nullString) {
		return 0;
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
	}

	else {
		return -1;
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		
	}

	return 0;
}

int newDevice(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring serialTag, std::wstring deviceModelId, std::wstring purchaseDate, std::wstring deviceCost, std::wstring operatingSystem, bool isTestServer) {
	/*
	*
		Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
		it will be issued.

	*/


	//Bind parameters
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialTag.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)purchaseDate.c_str(), 0, &strlen);
	SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceCost.c_str(), 0, &strlen);
	SQLRETURN bindResult6 = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)operatingSystem.c_str(), 0, &strlen);

	//Generate query
	std::wstring addDeviceQuery = L"INSERT INTO Devices (Device_Name, Serial_Tag, Device_Model_ID, Date_Purchased, Cost, Operating_System) VALUES (?, ?, ?, ?, ?, ?)";

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

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)purchaseDate.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Devices SET Date_Purchased = RTRIM(Date_Purchased) WHERE Date_Purchased = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)operatingSystem.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Devices SET Date_Purchased = RTRIM(Date_Purchased) WHERE Date_Purchased = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	//Free parameters
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	// Get Name aand Tag From DB
	SQLRETURN bindName = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	std::wstring statementResult;
	std::wstring checkNameResultQuery = L"SELECT TOP (1) [Device_Name] FROM [Devices] WHERE Device_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkNameResultQuery.c_str(), SQL_NTS);

	std::wstring checkName = getResult(hStmt, 1);

	std::wstring checkTagResultQuery = L"SELECT TOP (1) [Serial_Tag] FROM [Devices] WHERE Device_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkTagResultQuery.c_str(), SQL_NTS);

	std::wstring checkTag = getResult(hStmt, 1);

	// Copy and reformat values to compare against
	std::wstring deviceNumberFromDB = deviceNumber;
	std::wstring deviceSerialFromDB = serialTag;

	deviceNumberFromDB.resize(64);
	deviceSerialFromDB.resize(64);


	if (checkName == deviceNumberFromDB && checkTag == deviceSerialFromDB) {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else {
		std::cout << std::endl << "FAILED" << std::endl << std::endl;
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		diagSQLError(SQL_HANDLE_STMT, hStmt);
		enterKey();
		return -1;
	}
}

int addEmployee(SQLHSTMT hStmt, std::wstring employeeName, std::wstring employeeTitle, std::wstring employeeLocation, std::wstring employeeEmailAddress, std::wstring employeePhoneNumber, std::wstring employeeExtension) {
	/*

		Add an employee to the database.

	*/
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

	bool isValid = checkValid(hStmt, L"[Employees]", L"[Employee_ID]", L"Employee_ID", getIdFromEmail(hStmt, employeeEmailAddress));

	if (isValid) 
	{
		return 0;
	}
	else 
	{
		return -1;
	}
	
}

int removeEmployee(SQLHSTMT hStmt, std::wstring employeeID)
{
	/*
	*
		Remove an employee from the database.

		PAUSE ON ERROR

	*/

	SQLLEN strlen = SQL_NTS;

	// Rebind after while loop (parameter is not allowed out of scope)
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employeeID.c_str(), 0, &strlen);

	// Unassign device
	std::wstring unassignAllDevices = L"UPDATE [Devices] SET Currently_Issued_to = NULL WHERE Currently_Issued_to = ?";
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);


	// Finally, remove the employee
	std::wstring removeEmpQuery = L"DELETE FROM [Employees] WHERE Employee_ID = ?";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)removeEmpQuery.c_str(), SQL_NTS);

	bool isValid = checkValid(hStmt, L"[Employees]", L"[Employee_ID]", L"Employee_ID", employeeID);

	if (!isValid)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}
	
}

int removeDevice(SQLHSTMT hStmt, std::wstring deviceNumber) {
	/*

		Remove a device from the database.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	//Delete the device
	std::wstring unassignDevice = L"DELETE FROM [Devices] WHERE Device_Name = ?";
	SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	bool isValid = checkValid(hStmt, L"[Devices]", L"[Device_Name]", L"Device_Name", deviceNumber);

	if (!isValid)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}
}

void writeLastDevice(std::wstring deviceNumber) {

	std::wstring line[] = { L"", L"", L"" };
	std::wfstream file;

	file.open("config.cfg", std::ios::in);

	if (file.is_open()) {

		for (int i = 0; i < 3; i++) {

			std::getline(file, line[i]);

		}

	}

	else {
		std::cout << "Error opening file. Check if the config file is present.";
		return;
	}



	//write the last device to the last device file
	std::wstring filePath = line[2] + L"\\LastDevice.txt";
	std::wofstream lastDevice(filePath);
	lastDevice << deviceNumber;
	lastDevice.close();

	return;
}

void readLastDevice() {
	std::wstring line[] = { L"", L"", L"" };
	std::wfstream file;

	file.open("config.cfg", std::ios::in);

	if (file.is_open()) {

		for (int i = 0; i < 3; i++) {

			std::getline(file, line[i]);

		}

	}

	else {
		std::cout << "Error opening file. Check if the config file is present.";
		return;
	}



	//write the last device to the last device file
	std::wstring filePath = line[2] + L"\\LastDevice.txt";

	//Get the last device
	std::wstring deviceNumber;
	std::wifstream lastDevice(filePath);
	lastDevice >> deviceNumber;
	lastDevice.close();

	std::wcout << std::endl << L"Last device entered: " + deviceNumber << std::endl << std::endl;

	return;
}

int connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt, bool& isTestServer) {
	/*

		 Connects the application to the database. THe connection string is read from config.cfg.

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
	char dbChoice;
	std::cin >> dbChoice;

	if (dbChoice == 'C') {
		std::cout << "This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU Affero General Public License for more details. You should have received a copy of the GNU Affero General Public License along with this program.If not, see <https://www.gnu.org/licenses/>." << std::endl << std::endl;

		std::cout << "ENTER '1' FOR TEST SERVER. ENTER '2' FOR LIVE SERVER." << std::endl;

		std::cin >> dbChoice;
	}

	std::wstring connectionString = getConString(dbChoice, isTestServer);

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

std::wstring getConString(char dbChoice, bool &isTestServer) {
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
		isTestServer = true;
		return line[0];
	}
	else if (dbChoice == '2') {
		isTestServer = false;
		return line[1];
	}
	else {
		return L"-1";
	}
}

SQLRETURN diagSQLError(int sqlHandle, SQLHANDLE handle) {
	/*

		Sets up SQLGetDiagRec() and prints debug mesages to the terminal.

		Bug: appears to only be able to print once per program execution.

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

std::wstring getIdFromEmail(SQLHANDLE hStmt, std::wstring employeeEmail) {
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
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)getIdQuery.c_str(), SQL_NTS);

	if (statementResult != 0 and statementResult != 1) {
		diagSQLError(SQL_HANDLE_STMT, hStmt);
	}
	std::wstring idString = getResult(hStmt, 1);

	return idString;
}

std::wstring getLocationFromID(SQLHANDLE hStmt, std::wstring locationID) {
	/*
		This function takes the statement handle and location ID and returns the name of the location from the database.
	*/


	// Bind Location ID to param 1
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)locationID.c_str(), 0, &strlen);

	std::wstring getIdQuery = L"SELECT TOP (1) [Location_Name] FROM Locations WHERE Location_ID = ?";

	// Execute the query
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)getIdQuery.c_str(), SQL_NTS);

	if (statementResult != 0 and statementResult != 1) {
		diagSQLError(SQL_HANDLE_STMT, hStmt);
	}

	std::wstring locationName = getResult(hStmt, 1);

	return locationName;
}

std::wstring getResult(SQLHANDLE hStmt, int column) {
	/*

		Return the result from a select query. The first input, handle, is the statement handle. The second input points the ODBC
		cursor to the correct column.

	*/

	// Set the cursor to the first row of results
	SQLRETURN fetchReturn = SQLFetch(hStmt);

	// Set up result variables 
	SQLRETURN getResult;
	std::wstring getResultString;

	//Set string buffer
	getResultString.resize(64);
	SQLLEN stringLength = 64;
	SQLLEN* stringLengthPtr = &stringLength;

	// Get the data in the column passed to the function
	getResult = SQLGetData(hStmt, column, SQL_C_WCHAR, (void*)getResultString.data(), getResultString.size(), stringLengthPtr);

	// Return an error
	if (getResult == -1) {
		SQLCloseCursor(hStmt);
		return L"-1";
	}

	//Close cursor so that more queries can run
	SQLCloseCursor(hStmt);

	return getResultString;
}

void enterKey() {
	/*

		Quick function to pause the program and allow the user to read important information.

	*/
	char key;
	std::cout << "Enter any character to continue" << std::endl;
	while (true) {
		std::cin >> key;
		std::cout << std::endl;
		break;
	}
	return;
}

bool checkValid(SQLHANDLE hStmt, std::wstring table, std::wstring column, std::wstring param, std::wstring target) {
	/*
		EXAMPLE: "SELECT TOP (1) [Issued_New] FROM [Devices] WHERE Device_Name = ?";
	*/

	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1;
	SQLRETURN execResult;
	std::wstring checkResult;

	// Bind parameter
	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)target.c_str(), 0, &strlen);

	// Check that this device exists
	std::wstring checkDevice = L"SELECT TOP (1) " + column + L" FROM " + table + L" WHERE " + param + L" = ?";
	execResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDevice.c_str(), SQL_NTS);

	checkResult = getResult(hStmt, 1);

	if (checkResult == L"-1") {
		return false;
	}

	else {
		return true;
	}

}

