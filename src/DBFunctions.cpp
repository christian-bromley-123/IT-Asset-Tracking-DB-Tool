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
#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <vector>

#include <wchar.h>



int assignDevice(SQLHSTMT hStmt, std::wstring deviceNumber , std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser) {
	/*

		Assigns an existing device to an employee. The device name and employee email address is provided by the user.

	*/

	// Verify inputs
	bool dateValid = isValidDate(issueDate);

	if (!dateValid)
	{
		return -1;
	}


	while (true) 
	{
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

	SQLRETURN updateStatementResult = SQLExecute(hStmt);

	//Rebind
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	// Check query result
	std::wstring checkExecResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Devices] WHERE Device_Name = ?";
	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkExecResultQuery.c_str(), SQL_NTS);

	std::wstring checkExecResult = getResult(hStmt, 1);

	std::wstring nullString = L"";

	if (checkExecResult == nullString && (updateStatementResult != -1 && updateStatementResult != 100)) {
		return 0;
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
	}

	else {
		return -1;
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		
	}

}

int newDevice(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring serialTag, std::wstring deviceModelId, std::wstring purchaseDate, std::wstring deviceCost, std::wstring operatingSystem, bool isTestServer) {
	/*
	*
		Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
		it will be issued.

	*/

	// Verify inputs
	bool dateValid = isValidDate(purchaseDate);

	if (!dateValid)
	{
		return -1;
	}


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

	if (result != 0 && result != 1)
	{	
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return - 1;
	}

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


	if (checkName == deviceNumberFromDB && checkTag == deviceSerialFromDB) {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}
}

int addEmployee(SQLHSTMT hStmt, std::wstring employeeName, std::wstring employeeTitle, std::wstring employeeLocationId, std::wstring employeeEmailAddress, std::wstring employeePhoneNumber, std::wstring employeeExtension) {
	/*

		Add an employee to the database.

	*/

	std::vector<int> bindBuffers;

	bindBuffers.push_back(employeeName.size());
	bindBuffers.push_back(employeeTitle.size());
	bindBuffers.push_back(employeeLocationId.size());
	bindBuffers.push_back(employeeEmailAddress.size());
	bindBuffers.push_back(employeePhoneNumber.size());
	bindBuffers.push_back(employeeExtension.size());

	//Bind parameters
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, bindBuffers[0], 0, (wchar_t*)employeeName.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, bindBuffers[1], 0, (wchar_t*)employeeTitle.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, bindBuffers[2], 0, (wchar_t*)employeeLocationId.c_str(), 0, &strlen);
	SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, bindBuffers[3], 0, (wchar_t*)employeeEmailAddress.c_str(), 0, &strlen);
	SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, bindBuffers[4], 0, (wchar_t*)employeePhoneNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult6 = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, bindBuffers[5], 0, (wchar_t*)employeeExtension.c_str(), 0, &strlen);

	//Exectution string
	std::wstring newEmployeeString = L"INSERT INTO Employees(Employee_Name, Employee_Title, Location_ID, Employee_Email, Employee_Phone_Number, Employee_Ext) VALUES (?, ?, ?, ?, ?, ?)";

	//Execute
	int retcode = SQLExecDirect(hStmt, (SQLWCHAR*)newEmployeeString.c_str(), SQL_NTS);

	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	bool isValid = checkValid(hStmt, L"[Employees]", L"[Employee_ID]", L"Employee_ID", getIdFromEmail(hStmt, employeeEmailAddress));

	if (isValid && (retcode == 0 || retcode == 1)) 
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

	if (!isValid && (statementResult != -1 && statementResult != 100))
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

	if (!isValid && (deleteResult != -1 && deleteResult != 100))
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

int connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt, char dbChoice, bool& isTestServer) {
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

	std::wstring connectionString = getConString(dbChoice, isTestServer);

	result = SQLDriverConnect(hDbc, NULL, (SQLWCHAR*)connectionString.c_str(), SQL_NTS, NULL, 1, NULL, SQL_DRIVER_COMPLETE);

	if (result != 0 and result != 1) {
		return -1;
	}

	SQLRETURN stmtHandleReturn = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	

	//Set hStmt attribute
	SQLRETURN stmtAttrReturn = SQLSetStmtAttr(hStmt, SQL_ATTR_CURSOR_SCROLLABLE, (void*)SQL_SCROLLABLE, 0);

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
	SQLRETURN bindResult;
	bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)locationID.c_str(), 0, &strlen);

	std::wstring getIdQuery = L"SELECT TOP (1) [Location_Name] FROM Locations WHERE Location_ID = ?";

	// Execute the query
	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)getIdQuery.c_str(), SQL_NTS);

	if (statementResult != 0 and statementResult != 1) {
		diagSQLError(SQL_HANDLE_STMT, hStmt);
	}

	std::wstring locationName = getResult(hStmt, 1);

	return locationName;
}

std::wstring getLocationIDFromName(SQLHANDLE hStmt, std::wstring locationName) {
	/* 
		Function to return the location ID given the name of the location.
	*/

	SQLLEN strlen = SQL_NTS;

	// Bind LocationName to param 1
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)locationName.c_str(), 0, &strlen);

	std::wstring getLocationQuery = L"SELECT TOP (1) [Location_ID] FROM Locations WHERE Location_Name = ?";

	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)getLocationQuery.c_str(), SQL_NTS);

	std::wstring locationID = getResult(hStmt, 1);

	return locationID;

}

std::wstring getModelIdFromName(SQLHANDLE hStmt, std::wstring deviceName)
{
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceName.c_str(), 0, &strlen);

	std::wstring getModelIdQuery = L"SELECT TOP (1) [Device_Model_ID] FROM [Device_Models] WHERE Device_Model_Name = ?";
	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)getModelIdQuery.c_str(), SQL_NTS);
	
	return getResult(hStmt);

}

std::wstring getResult(SQLHANDLE hStmt, int column, int row, bool lastResult)
{
	std::vector<std::wstring> getResultVector = getResultRow(hStmt, row);
	
	return getResultVector[column-1];
}

std::vector<std::wstring> getResultRow(SQLHANDLE hStmt, int row, bool lastResult) 
{
	/*

		Return the result from a select query. The first input, handle, is the statement handle. The second input points the ODBC
		cursor to the correct column.

	*/

	// Get size the size needed for the result vector
	short numColumns;
	SQLNumResultCols(hStmt, &numColumns);

	// Declare and resize the result vector an dlength vector
	std::vector<std::wstring> getResultVector;
	std::vector<SQLLEN> stringLengthVector;

	getResultVector.resize(numColumns);
	stringLengthVector.resize(numColumns);

	// Set string buffer for each element of the result vector
	for (int i=0;i<numColumns;i++) 
	{
		getResultVector[i].resize(64);
		stringLengthVector[i] = 64;
	}
	
	// Bind result columns for Fetch
	for (int i=0; i<(int)numColumns;i++)
	{
		SQLRETURN retBinds = SQLBindCol(hStmt, i + 1, SQL_C_WCHAR, (void*)getResultVector[i].data(), stringLengthVector[i], &stringLengthVector[i]);
	}

	// Fetch the row on the provided column
	SQLRETURN fetchReturn = SQLFetchScroll(hStmt, SQL_FETCH_ABSOLUTE, row);

	//Resize results
	for (int i=0; i < (int)numColumns;i++)
	{
		// SQLFetchScroll will not change stringLengthVector[i] if it returns 100 (No data)
		if (stringLengthVector[i] == -1 or fetchReturn == SQL_NO_DATA)
		{
			getResultVector[i].resize(0);
		}

		else
		{
			// the size of a wchar_t is 2 bytes. StringLengthVector will have returned the number of bytes, which is not what we want, hence division by 2
			getResultVector[i].resize(stringLengthVector[i]/2);
		}
	}

	//Close cursor so that more queries can run
	if (lastResult) 
	{
		SQLCloseCursor(hStmt);
	}

	return getResultVector;
}

std::vector<std::wstring> getColumn (SQLHANDLE hStmt, bool isDistinct, std::wstring table, std::wstring column, std::wstring param, std::wstring target)
{
	std::wstring distinct = L"";

	if (isDistinct) 
	{
		distinct = L"DISTINCT ";
	}

	std::wstring resultCountString;

	if (param == L"")
	{
		resultCountString = L"SELECT COUNT (" + distinct + column + L") FROM " + table;
	}

	else
	{
		// Bind target avoid type errors
		SQLLEN strlen = SQL_NTS;
		SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)target.c_str(), 0, &strlen);
		resultCountString = L"SELECT COUNT (" + distinct + column + L") FROM " + table + L" WHERE " + param + L" = ?";
	}
	

	// Execute the query to find size of result loop.
	SQLLEN strlen = SQL_NTS;
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)resultCountString.c_str(), SQL_NTS);

	std::wstring resultCountStr = getResult(hStmt, 1);

	wchar_t* resultCountStrEnd;

	int resultCountStringSize = resultCountStr.size();

	int resultCount = wcstol((wchar_t*)resultCountStr.c_str(), &resultCountStrEnd, 10);

	std::wstring resultStringQuery;

	if (param == L"")
	{
		resultStringQuery = L"SELECT " + distinct + column + L" FROM " + table;
	}

	else
	{
		resultStringQuery = L"SELECT " + distinct + column + L" FROM " + table + L" WHERE " + param + L" = ?";
	}

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)resultStringQuery.c_str(), SQL_NTS);

	std::vector<std::wstring> results;
	std::wstring nextResult;

	for (int i=0;i<resultCount;i++)
	{

		bool lastResult = false;

		if (i == resultCount-1) 
		{
			lastResult = true;
		}
		nextResult = getResult(hStmt, i+1, lastResult);
		results.push_back(nextResult);
	}

	

	return results;
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

	if (checkResult == L"") {
		return false;
	}

	else {
		return true;
	}
}

bool isValidDate(std::wstring date) 
{

	// First check the length, it should be 8 characters every time.
	if (date.length() != 8)
	{
		return false;
	}

	// Check to make sure the day, month, and year are delimited by slashes
	if (date[2] != L'/' or date[5] != L'/')
	{
		return false;
	}

	// cut up string into day month and year to check them individually
	std::wstring month = date.substr(0, 2);
	std::wstring day = date.substr(3, 2);
	std::wstring year = date.substr(6, 2);

	// Convert days, months, and years to numbers for comparison
	wchar_t* monthEnd;
	wchar_t* dayEnd;
	wchar_t* yearEnd;

	int monthInt = wcstol((wchar_t*)month.c_str(), &monthEnd, 10);
	int dayInt = wcstol((wchar_t*)day.c_str(), &dayEnd, 10);
	int yearInt = wcstol((wchar_t*)year.c_str(), &yearEnd, 10);

	if (monthInt > 12 or monthInt < 1)
	{
		return false;
	}

	if (monthInt == 4 || monthInt == 6 || monthInt == 9 || monthInt == 11)
	{
		if (dayInt > 30 || dayInt < 1)
		{
			return false;
		}

		else
		{
			return true;
		}
	}

	else if (monthInt == 2)
	{
		// check if it is a leap year
		if (dayInt == 29)
		{
			// End-of-century years typically aren't leap years, but 2000 was. If this program is still in use in 2100 re-write this code... 
			// I would, but I'll be busy being dead.
			if (yearInt % 4 == 0)
			{
				return true;
			}

			// Not divisible by four means it is not a leap year
			else
			{
				return false;
			}
		}

		// Non-leap years
		else if (dayInt > 28 || dayInt < 1)
		{
			return false;
		}

		else
		{
			return true;
		}
	}

	else
	{
		if (dayInt > 31 || dayInt < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

