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

int assignComputer(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser) {
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
		std::wstring assignDeviceQuery = L"UPDATE [Computers] SET Currently_Issued_To = ?, Date_Issued = ?, Issued_New = ? WHERE Computer_Name = ?";

		//Execute
		SQLRETURN statementResult;
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)assignDeviceQuery.c_str(), SQL_NTS);

		//TRIM
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		std::wstring trimDateQuery = L"UPDATE Computers SET Date_Issued = RTRIM(Date_Issued) WHERE Computer_Name = ?";
		SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)trimDateQuery.c_str(), SQL_NTS);
		std::wstring trimNewQuery = L"UPDATE Computers SET Issued_New = RTRIM(Issued_New) WHERE Computer_Name = ?";
		result = SQLExecDirect(hStmt, (wchar_t*)trimNewQuery.c_str(), SQL_NTS);


		//Rebind
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check query result UPDATE TO INCLUDE ALL NEW INFO
		std::wstring checkUserResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Computers] WHERE Computer_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkUserResultQuery.c_str(), SQL_NTS);
		std::wstring checkUserResult = getResult(hStmt, 1);

		std::wstring checkDateResultQuery = L"SELECT TOP (1) [Date_Issued] FROM [Computers] WHERE Computer_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDateResultQuery.c_str(), SQL_NTS);
		std::wstring checkDateResult = getResult(hStmt, 1);

		std::wstring checkNewResultQuery = L"SELECT TOP (1) [Issued_New] FROM [Computers] WHERE Computer_Name = ?";
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

int assignPeripheral(SQLHSTMT hStmt, std::wstring deviceNumber , std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser) {
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
		std::wstring assignDeviceQuery = L"UPDATE [Peripherals] SET Currently_Issued_To = ?, Date_Issued = ?, Issued_New = ? WHERE Peripheral_Name = ?";

		//Execute
		SQLRETURN statementResult;
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)assignDeviceQuery.c_str(), SQL_NTS);

		//TRIM
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		std::wstring trimDateQuery = L"UPDATE Peripherals SET Date_Issued = RTRIM(Date_Issued) WHERE Peripheral_Name = ?";
		SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)trimDateQuery.c_str(), SQL_NTS);
		std::wstring trimNewQuery = L"UPDATE Peripherals SET Issued_New = RTRIM(Issued_New) WHERE Peripherale_Name = ?";
		result = SQLExecDirect(hStmt, (wchar_t*)trimNewQuery.c_str(), SQL_NTS);


		//Rebind
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check query result UPDATE TO INCLUDE ALL NEW INFO
		std::wstring checkUserResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Peripherals] WHERE Peripheral_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkUserResultQuery.c_str(), SQL_NTS);
		std::wstring checkUserResult = getResult(hStmt, 1);

		std::wstring checkDateResultQuery = L"SELECT TOP (1) [Date_Issued] FROM [Peripherals] WHERE Peripheral_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDateResultQuery.c_str(), SQL_NTS);
		std::wstring checkDateResult = getResult(hStmt, 1);

		std::wstring checkNewResultQuery = L"SELECT TOP (1) [Issued_New] FROM [Peripherals] WHERE Peripheral_Name = ?";
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

int assignHotspot(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate) {
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
		SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		//Generate Query
		std::wstring assignDeviceQuery = L"UPDATE [Hotspots] SET Currently_Issued_To = ?, Date_Issued = ? WHERE Phone_Number = ?";

		//Execute
		SQLRETURN statementResult;
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)assignDeviceQuery.c_str(), SQL_NTS);

		//TRIM
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		std::wstring trimDateQuery = L"UPDATE Hotspots SET Date_Issued = RTRIM(Date_Issued) WHERE Phone_Number = ?";
		SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)trimDateQuery.c_str(), SQL_NTS);
		std::wstring trimNewQuery = L"UPDATE Hotspots SET Issued_New = RTRIM(Issued_New) WHERE Phone_Number = ?";
		result = SQLExecDirect(hStmt, (wchar_t*)trimNewQuery.c_str(), SQL_NTS);


		//Rebind
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check query result UPDATE TO INCLUDE ALL NEW INFO
		std::wstring checkUserResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Hotspots] WHERE Phone_Number = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkUserResultQuery.c_str(), SQL_NTS);
		std::wstring checkUserResult = getResult(hStmt, 1);

		std::wstring checkDateResultQuery = L"SELECT TOP (1) [Date_Issued] FROM [Hotspots] WHERE Phone_Number = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDateResultQuery.c_str(), SQL_NTS);
		std::wstring checkDateResult = getResult(hStmt, 1);

		// Copy and reformat values to compare against
		std::wstring dateFromDB = issueDate;

		if (checkUserResult == employeeID && checkDateResult == dateFromDB) {
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
			return 0;
		}

		else {
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
			return -1;
		}
	}
}

int assignEquipment(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate) {
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
		SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		//Generate Query
		std::wstring assignDeviceQuery = L"UPDATE [Office_Equipment] SET Currently_Issued_To = ?, Date_Issued = ? WHERE Equipment_Name = ?";

		//Execute
		SQLRETURN statementResult;
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)assignDeviceQuery.c_str(), SQL_NTS);

		//TRIM
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		std::wstring trimDateQuery = L"UPDATE Office_Equipment SET Date_Issued = RTRIM(Date_Issued) WHERE Equipment_Name = ?";
		SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)trimDateQuery.c_str(), SQL_NTS);
		std::wstring trimNewQuery = L"UPDATE Office_Equipment SET Issued_New = RTRIM(Issued_New) WHERE Equipment_Name = ?";
		result = SQLExecDirect(hStmt, (wchar_t*)trimNewQuery.c_str(), SQL_NTS);


		//Rebind
		bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

		// Check query result UPDATE TO INCLUDE ALL NEW INFO
		std::wstring checkUserResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Office_Equipment] WHERE Equipment_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkUserResultQuery.c_str(), SQL_NTS);
		std::wstring checkUserResult = getResult(hStmt, 1);

		std::wstring checkDateResultQuery = L"SELECT TOP (1) [Date_Issued] FROM [Office_Equipment] WHERE Equipment_Name = ?";
		statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkDateResultQuery.c_str(), SQL_NTS);
		std::wstring checkDateResult = getResult(hStmt, 1);

		// Copy and reformat values to compare against
		std::wstring dateFromDB = issueDate;

		if (checkUserResult == employeeID && checkDateResult == dateFromDB) {
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
			return 0;
		}

		else {
			SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
			return -1;
		}
	}
}

int unassignComputer(SQLHSTMT hStmt, std::wstring deviceNumber) {

	/*

		Runs the query to unassign a device from a user, freeing it.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;

	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	std::wstring unassignDevice = L"UPDATE [Computers] SET Currently_Issued_To = NULL WHERE Computer_Name = ?";

	//Prepare and execute the query
	SQLRETURN prepareResult = SQLPrepare(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	SQLRETURN updateStatementResult = SQLExecute(hStmt);

	//Rebind
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	// Check query result
	std::wstring checkExecResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Computers] WHERE Computer_Name = ?";
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

int unassignPeripheral(SQLHSTMT hStmt, std::wstring deviceNumber) {

	/*

		Runs the query to unassign a device from a user, freeing it.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;

	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	std::wstring unassignDevice = L"UPDATE [Peripherals] SET Currently_Issued_To = NULL WHERE Peripheral_Name = ?";

	//Prepare and execute the query
	SQLRETURN prepareResult = SQLPrepare(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	SQLRETURN updateStatementResult = SQLExecute(hStmt);

	//Rebind
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	// Check query result
	std::wstring checkExecResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Peripherals] WHERE Peripheral_Name = ?";
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

int unassignHotspot(SQLHSTMT hStmt, std::wstring deviceNumber) {

	/*

		Runs the query to unassign a device from a user, freeing it.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;

	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	std::wstring unassignDevice = L"UPDATE [Hotspots] SET Currently_Issued_To = NULL WHERE Phone_Number = ?";

	//Prepare and execute the query
	SQLRETURN prepareResult = SQLPrepare(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	SQLRETURN updateStatementResult = SQLExecute(hStmt);

	//Rebind
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	// Check query result
	std::wstring checkExecResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Hotspots] WHERE Phone_Number = ?";
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

int unassignEquipment(SQLHSTMT hStmt, std::wstring deviceNumber) {

	/*

		Runs the query to unassign a device from a user, freeing it.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;

	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	std::wstring unassignDevice = L"UPDATE [Office_Equipment] SET Currently_Issued_To = NULL WHERE Equipment_Name = ?";

	//Prepare and execute the query
	SQLRETURN prepareResult = SQLPrepare(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	SQLRETURN updateStatementResult = SQLExecute(hStmt);

	//Rebind
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceNumber.c_str(), 0, &strlen);

	// Check query result
	std::wstring checkExecResultQuery = L"SELECT TOP (1) [Currently_Issued_To] FROM [Office_Equipment] WHERE Equipment_Name = ?";
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

int newComputer(SQLHSTMT hStmt, std::wstring computerName, std::wstring serialNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, std::wstring operatingSystem, bool isTestServer) {
	/*
	*
		Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
		it will be issued.

	*/

	// Verify inputs
	bool dateValid = isValidDate(datePurchased);

	if (!dateValid)
	{
		return -1;
	}


	//Bind parameters
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)computerName.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)cost.c_str(), 0, &strlen);
	SQLRETURN bindResult6 = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)operatingSystem.c_str(), 0, &strlen);

	//Generate query
	std::wstring addDeviceQuery = L"INSERT INTO Computers (Computer_Name, Serial_Number, Device_Model_ID, Date_Purchased, Cost, Operating_System) VALUES (?, ?, ?, ?, ?, ?)";
	
	//Execute 
	SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	if (result != 0 && result != 1)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}

	// Free binds
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);


	//THis program tends to put a lot of space characters to the right
	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)computerName.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Computers SET Computer_Name = RTRIM(Computer_Name) WHERE Computer_Name = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialNumber.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Computers SET Serial_Number = RTRIM(Serial_Number) WHERE Serial_Number = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Computers SET Device_Model_ID = RTRIM(Device_Model_ID) WHERE Device_Model_ID = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Computers SET Date_Purchased = RTRIM(Date_Purchased) WHERE Date_Purchased = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)operatingSystem.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Computers SET Operating_System = RTRIM(Operating_System) WHERE Operating_System = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	//Free parameters
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	// Get Name aand Tag From DB
	SQLRETURN bindName = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)computerName.c_str(), 0, &strlen);

	std::wstring statementResult;
	std::wstring checkNameResultQuery = L"SELECT TOP (1) [Computer_Name] FROM [Computers] WHERE Computer_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkNameResultQuery.c_str(), SQL_NTS);

	std::wstring checkName = getResult(hStmt, 1);

	std::wstring checkTagResultQuery = L"SELECT TOP (1) [Serial_Number] FROM [Computers] WHERE Computer_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkTagResultQuery.c_str(), SQL_NTS);

	std::wstring checkTag = getResult(hStmt, 1);

	// Copy and reformat values to compare against
	std::wstring deviceNumberFromDB = computerName;
	std::wstring deviceSerialFromDB = serialNumber;

	if (checkName == deviceNumberFromDB && checkTag == deviceSerialFromDB) {
		int transactionResult = recordDeviceTransation(hStmt, L"Test_Device", L"1");
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}
}

int newPeripheral(SQLHSTMT hStmt, std::wstring peripheralName, std::wstring serialNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, bool isTestServer) {
	/*
	*
		Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
		it will be issued.

	*/

	// Verify inputs
	bool dateValid = isValidDate(datePurchased);

	if (!dateValid)
	{
		return -1;
	}


	//Bind parameters
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)peripheralName.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)cost.c_str(), 0, &strlen);

	//Generate query
	std::wstring addDeviceQuery = L"INSERT INTO Peripherals (Peripheral_Name, Serial_Number, Device_Model_ID, Date_Purchased, Cost) VALUES (?, ?, ?, ?, ?)";

	//Execute 
	SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	if (result != 0 && result != 1)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}

	// Free binds
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);


	//THis program tends to put a lot of space characters to the right
	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)peripheralName.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Peripherals SET Peripheral_Name = RTRIM(Peripheral_Name) WHERE Peripheral_Name = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialNumber.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Peripherals SET Serial_Number = RTRIM(Serial_Number) WHERE Serial_Number = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Peripherals SET Device_Model_ID = RTRIM(Device_Model_ID) WHERE Device_Model_ID = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Peripherals SET Date_Purchased = RTRIM(Date_Purchased) WHERE Date_Purchased = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	//Free parameters
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	// Get Name aand Tag From DB
	SQLRETURN bindName = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)peripheralName.c_str(), 0, &strlen);

	std::wstring statementResult;
	std::wstring checkNameResultQuery = L"SELECT TOP (1) [Peripheral_Name] FROM [Peripherals] WHERE Peripheral_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkNameResultQuery.c_str(), SQL_NTS);

	std::wstring checkName = getResult(hStmt, 1);

	std::wstring checkTagResultQuery = L"SELECT TOP (1) [Serial_Number] FROM [Peripherals] WHERE Peripheral_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkTagResultQuery.c_str(), SQL_NTS);

	std::wstring checkTag = getResult(hStmt, 1);

	// Copy and reformat values to compare against
	std::wstring deviceNumberFromDB = peripheralName;
	std::wstring deviceSerialFromDB = serialNumber;


	if (checkName == deviceNumberFromDB && checkTag == deviceSerialFromDB) {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}
}

int newHotspot(SQLHSTMT hStmt, std::wstring phoneNumber, std::wstring imeiNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, bool isTestServer) {
	/*
	*
		Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
		it will be issued.

	*/

	// Verify inputs
	bool dateValid = isValidDate(datePurchased);

	if (!dateValid)
	{
		return -1;
	}


	//Bind parameters
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)phoneNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)imeiNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)cost.c_str(), 0, &strlen);

	//Generate query
	std::wstring addDeviceQuery = L"INSERT INTO Hotspots (Phone_Number, IMEI, Device_Model_ID, Date_Purchased, Cost) VALUES (?, ?, ?, ?, ?)";

	//Execute 
	SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	if (result != 0 && result != 1)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}

	// Free binds
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);


	//THis program tends to put a lot of space characters to the right
	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)phoneNumber.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Hotspots SET Phone_Number = RTRIM(Phone_Number) WHERE Phone_Number = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)imeiNumber.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Hotspots SET IMEI = RTRIM(IMEI) WHERE IMEI = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Hotspots SET Device_Model_ID = RTRIM(Device_Model_ID) WHERE Device_Model_ID = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Hotspots SET Date_Purchased = RTRIM(Date_Purchased) WHERE Date_Purchased = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	//Free parameters
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	// Get Name aand Tag From DB
	SQLRETURN bindName = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)phoneNumber.c_str(), 0, &strlen);

	std::wstring statementResult;
	std::wstring checkNameResultQuery = L"SELECT TOP (1) [Phone_Number] FROM [Hotspots] WHERE Phone_Number = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkNameResultQuery.c_str(), SQL_NTS);

	std::wstring checkName = getResult(hStmt, 1);

	std::wstring checkTagResultQuery = L"SELECT TOP (1) [IMEI] FROM [Hotspots] WHERE Phone_Number = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkTagResultQuery.c_str(), SQL_NTS);

	std::wstring checkTag = getResult(hStmt, 1);

	// Copy and reformat values to compare against
	std::wstring deviceNumberFromDB = phoneNumber;
	std::wstring deviceSerialFromDB = imeiNumber;


	if (checkName == deviceNumberFromDB && checkTag == deviceSerialFromDB) {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return 0;
	}

	else {
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}
}

int newEquipment(SQLHSTMT hStmt, std::wstring equipmentName, std::wstring serialNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, bool isTestServer) {
	/*
	*
		Enter a new device into the data base. The program will ask for its name, serial number, model, who it's going to be issued to, and the date
		it will be issued.

	*/

	// Verify inputs
	bool dateValid = isValidDate(datePurchased);

	if (!dateValid)
	{
		return -1;
	}


	//Bind parameters
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)equipmentName.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialNumber.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	SQLRETURN bindResult4 = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	SQLRETURN bindResult5 = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)cost.c_str(), 0, &strlen);

	//Generate query
	std::wstring addDeviceQuery = L"INSERT INTO Office_Equipment (Equipment_Name, Serial_Number, Device_Model_ID, Date_Purchased, Cost) VALUES (?, ?, ?, ?, ?)";

	//Execute 
	SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	if (result != 0 && result != 1)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}

	// Free binds
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);


	//THis program tends to put a lot of space characters to the right
	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)equipmentName.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Office_Equipment SET Equipment_Name = RTRIM(Equipment_Name) WHERE Equipment_Name = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)serialNumber.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Office_Equipment SET Serial_Number = RTRIM(Serial_Number) WHERE Serial_Number = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Office_Equipment SET Device_Model_ID = RTRIM(Device_Model_ID) WHERE Device_Model_ID = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)datePurchased.c_str(), 0, &strlen);
	addDeviceQuery = L"UPDATE Office_Equipment SET Date_Purchased = RTRIM(Date_Purchased) WHERE Date_Purchased = ?";
	result = SQLExecDirect(hStmt, (wchar_t*)addDeviceQuery.c_str(), SQL_NTS);

	//Free parameters
	SQLFreeStmt(hStmt, SQL_RESET_PARAMS);

	// Get Name aand Tag From DB
	SQLRETURN bindName = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)equipmentName.c_str(), 0, &strlen);

	std::wstring statementResult;
	std::wstring checkNameResultQuery = L"SELECT TOP (1) [Equipment_Name] FROM [Office_Equipment] WHERE Equipment_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkNameResultQuery.c_str(), SQL_NTS);

	std::wstring checkName = getResult(hStmt, 1);

	std::wstring checkTagResultQuery = L"SELECT TOP (1) [Serial_Number] FROM [Office_Equipment] WHERE Equipment_Name = ?";

	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)checkTagResultQuery.c_str(), SQL_NTS);

	std::wstring checkTag = getResult(hStmt, 1);

	// Copy and reformat values to compare against
	std::wstring deviceNumberFromDB = equipmentName;
	std::wstring deviceSerialFromDB = serialNumber;


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

	// Unassign devices
	std::wstring unassignAllDevices = L"UPDATE [Computers] SET Currently_Issued_to = NULL WHERE Currently_Issued_to = ?";
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	unassignAllDevices = L"UPDATE [Peripherals] SET Currently_Issued_to = NULL WHERE Currently_Issued_to = ?";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	unassignAllDevices = L"UPDATE [Hotspots] SET Currently_Issued_to = NULL WHERE Currently_Issued_to = ?";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	unassignAllDevices = L"UPDATE [Office_Equipment] SET Currently_Issued_to = NULL WHERE Currently_Issued_to = ?";
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

int removeComputer(SQLHSTMT hStmt, std::wstring computerName) {
	/*

		Remove a device from the database.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)computerName.c_str(), 0, &strlen);

	//Delete the device
	std::wstring unassignDevice = L"DELETE FROM [Computers] WHERE Computer_Name = ?";
	SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	bool isValid = checkValid(hStmt, L"[Computers]", L"[Computer_Name]", L"Computer_Name", computerName);

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

int removePeripheral(SQLHSTMT hStmt, std::wstring peripheralName) {
	/*

		Remove a device from the database.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)peripheralName.c_str(), 0, &strlen);

	//Delete the device
	std::wstring unassignDevice = L"DELETE FROM [Peripherals] WHERE Peripheral_Name = ?";
	SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	bool isValid = checkValid(hStmt, L"[Peripherals]", L"[Peripheral_Name]", L"Peripheral_Name", peripheralName);

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

int removeHotspot(SQLHSTMT hStmt, std::wstring hotspotName) {
	/*

		Remove a device from the database.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)hotspotName.c_str(), 0, &strlen);

	//Delete the device
	std::wstring unassignDevice = L"DELETE FROM [Hotspots] WHERE Phone_Number = ?";
	SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	bool isValid = checkValid(hStmt, L"[Hotspots]", L"[Phone_Number]", L"Phone_Number", hotspotName);

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

int removeEquipment(SQLHSTMT hStmt, std::wstring equipmentName) {
	/*

		Remove a device from the database.

	*/

	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)equipmentName.c_str(), 0, &strlen);

	//Delete the device
	std::wstring unassignDevice = L"DELETE FROM [Office_Equipment] WHERE Equipment_Name = ?";
	SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDevice.c_str(), SQL_NTS);

	bool isValid = checkValid(hStmt, L"[Office_Equipment]", L"[Equipment_Name]", L"Equipment_Name", equipmentName);

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

int writeReport(SQLHANDLE hStmt, int option)
{
	if (option == 1)
	{

		//Get number of results
		std::wstring reportOneCount = L"SELECT COUNT [Device_Name] as [Phone Number], [Serial_Tag] as [IMEI], [Employee_Name] as [Checked Out To:], [Employee_Email] as [Email], [Date_Issued] as [Date Issued], [Cost] as [Hotspot Cost] FROM [Devices] JOIN [Device_Models] ON [Device_Models].[Device_Model_ID] = [Devices].[Device_Model_ID] LEFT JOIN [Employees] ON [Employees].[Employee_ID] = [Devices].[Currently_Issued_To] WHERE Device_Model_Type = 'Hotspot'";
		SQLRETURN retcode = SQLExecDirect(hStmt, (SQLWCHAR*)reportOneCount.c_str(), SQL_NTS);



		std::wstring reportOneQuery = L"SELECT [Device_Name] as [Phone Number], [Serial_Tag] as [IMEI], [Employee_Name] as [Checked Out To:], [Employee_Email] as [Email], [Date_Issued] as [Date Issued], [Cost] as [Hotspot Cost] FROM [Devices] JOIN [Device_Models] ON [Device_Models].[Device_Model_ID] = [Devices].[Device_Model_ID] LEFT JOIN [Employees] ON [Employees].[Employee_ID] = [Devices].[Currently_Issued_To] WHERE Device_Model_Type = 'Hotspot'";
		retcode  = SQLExecDirect(hStmt, (SQLWCHAR*)reportOneQuery.c_str(), SQL_NTS);

		// Get size the size needed for the result vector
		short numColumns;
		SQLNumResultCols(hStmt, &numColumns);

		std::vector<std::wstring> columnNames = getAllResultColumnNames(hStmt);

		std::wstring reportPath = L"ReportOne.csv";

		std::wofstream reportOne(reportPath);

		for (int i=0;i<numColumns;i++)
		{
			if (i != numColumns-1)
			{
				reportOne << columnNames[i] << L",";
			}
			else
			{
				reportOne << columnNames[i] << std::endl;
			}
			
		}

		long long rowCount;
		SQLRowCount(hStmt, &rowCount);
		
		for (int i=0;i<rowCount;i++)
		{
			bool lastResult = false;

			if (i == rowCount-1)
			{
				lastResult = true;
			}

			std::vector<std::wstring> resultsRow = getResultRow(hStmt, i+1, lastResult);

			for (int j = 0; j < numColumns; j++)
			{
				if (j != numColumns - 1)
				{
					reportOne << resultsRow[j] << L",";
				}
				else
				{
					reportOne << L"$" << resultsRow[j] << std::endl;
				}
			}
		}

		reportOne.close();
		
		return 0;
	}

	return 0;
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

std::wstring getModelIdFromDeviceName(SQLHANDLE hStmt, std::wstring deviceName)
{	
	//Binding deviceNumber to the first parameter
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceName.c_str(), 0, &strlen);

	std::wstring query = L"SELECT [Device_Model_ID] FROM [Computers] WHERE Computer_Name = ?";
	SQLRETURN queryResult = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);


	if (getResult(hStmt, 1, 1, false) != L"")
	{
		return getResult(hStmt);
	}

	SQLCloseCursor(hStmt);
	query = L"SELECT [Device_Model_ID] FROM [Peripherals] WHERE Peripheral_Name = ?";
	queryResult = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (getResult(hStmt, 1, 1, false) != L"")
	{
		return getResult(hStmt);
	}

	SQLCloseCursor(hStmt);
	query = L"SELECT [Device_Model_ID] FROM [Hotspots] WHERE Phone_Number = ?";
	queryResult = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (getResult(hStmt, 1, 1, false) != L"")
	{
		return getResult(hStmt);
	}

	SQLCloseCursor(hStmt);
	query = L"SELECT [Device_Model_ID] FROM [Office_Equipment] WHERE Equipment_Name = ?";
	queryResult = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (getResult(hStmt, 1, 1, false) != L"")
	{
		return getResult(hStmt);
	}

	return L"";
}

std::wstring getModelIdFromModelName(SQLHANDLE hStmt, std::wstring modelName)
{
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)modelName.c_str(), 0, &strlen);

	std::wstring getModelIdQuery = L"SELECT TOP (1) [Device_Model_ID] FROM [Device_Models] WHERE Device_Model_Name = ?";
	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)getModelIdQuery.c_str(), SQL_NTS);
	
	return getResult(hStmt);

}

std::wstring getResult(SQLHANDLE hStmt, int column, int row, bool lastResult)
{
	std::vector<std::wstring> getResultVector = getResultRow(hStmt, row, lastResult);
	std::vector<std::wstring> emptyString;
	if (getResultVector.size() == 0)
	{
		return L"";
	}
	std::wstring getResultString = getResultVector[column - 1];
	return getResultString;
}

std::wstring getDeviceTypeFromModelId(SQLHANDLE hStmt, std::wstring deviceModelId)
{

	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);

	std::wstring query = L"SELECT [Device_Model_Type] FROM [Device_Models] WHERE [Device_Model_ID] = " + deviceModelId;
	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	return getResult(hStmt);
}

std::wstring getDeviceTableFromModelId(SQLHANDLE hStmt, std::wstring deviceModelId)
{
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceModelId.c_str(), 0, &strlen);

	std::wstring query = L"SELECT [Device_Model_Table] FROM [Device_Models] WHERE [Device_Model_ID] = " + deviceModelId;
	SQLRETURN statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	return getResult(hStmt);
}

std::vector<std::wstring> getResultRow(SQLHANDLE hStmt, int row, bool lastResult) 
{
	/*

		Return the result from a select query. The first input, handle, is the statement handle. The second input points the ODBC
		cursor to the correct column.

	*/

	// Get size the size needed for the result vector
	short numColumns = -1;
	SQLNumResultCols(hStmt, &numColumns);

	if (numColumns < 1)
	{
		std::vector<std::wstring> emptyVector;
		return emptyVector;
	}

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
			getResultVector[i].resize(wcslen((wchar_t*)getResultVector[i].data()));
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
	std::wstring nextResult = L"";

	for (int i=0;i<resultCount;i++)
	{

		bool lastResult = false;

		if (i == resultCount-1) 
		{
			lastResult = true;
		}
		nextResult = getResult(hStmt, 1, i+1, lastResult);
		results.push_back(nextResult);
	}
	return results;
}

std::wstring getResultColumnName(SQLHANDLE hStmt, int column)
{		

	std::wstring getColumnString;
	short outputBuffer = 64;
	getColumnString.resize(outputBuffer);
	short nameLength;
	short dataType;
	unsigned long long columnSize;
	short decimalDigits;
	short nullable;
	SQLLEN numAttr = 0;



	SQLDescribeCol(hStmt, column, (wchar_t*)getColumnString.c_str(), outputBuffer, &nameLength, &dataType, &columnSize, &decimalDigits, &nullable);

	getColumnString.resize(nameLength);

	return getColumnString;
}

std::vector<std::wstring> getAllResultColumnNames(SQLHANDLE hStmt)
{


	// Get size the size needed for the result vector
	short numColumns;
	SQLNumResultCols(hStmt, &numColumns);

	std::vector<std::wstring> columnNames;

	for (int i=1;i < numColumns+1; i++)
	{
		columnNames.push_back(getResultColumnName(hStmt, i));
	}

	return columnNames;
}

int recordDeviceTransation(SQLHANDLE hStmt, std::wstring deviceName, std::wstring transactionType, std::wstring employee)
{
	SQLLEN strlen = SQL_NTS;
	SQLRETURN bindResult1 = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)deviceName.c_str(), 0, &strlen);
	SQLRETURN bindResult2 = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)transactionType.c_str(), 0, &strlen);
	SQLRETURN bindResult3 = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 100, 0, (wchar_t*)employee.c_str(), 0, &strlen);

	std::wstring newDeviceTransaction;

	if (employee == L"")
	{
		newDeviceTransaction = L"INSERT INTO Device_Transactions (Device_Name, Transaction_Type, Employee) VALUES (?, ?, NULL)";
	}
	else 
	{
		newDeviceTransaction = L"INSERT INTO Device_Transactions (Device_Name, Transaction_Type, Employee) VALUES (?, ?, ?)";
	}
	

	//Execute 
	SQLRETURN result = SQLExecDirect(hStmt, (wchar_t*)newDeviceTransaction.c_str(), SQL_NTS);

	if (result != 0 && result != 1)
	{
		SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		return -1;
	}


	return 0;
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

