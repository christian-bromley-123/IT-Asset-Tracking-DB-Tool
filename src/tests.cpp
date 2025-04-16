#include "..\include\tests.hpp"
#include "..\include\DBFunctions.hpp"

#include <iostream>
#include <string>




std::vector<std::wstring> testAll(SQLHANDLE hStmt, bool isTestServer)
{
	/* Run a series of queries. Check the results */
	std::vector<std::wstring>retcodeStringVector;
	// Make sure the test server is selected
	if (!isTestServer)
	{
		std::vector<std::wstring> retcodeStringVector;
		return retcodeStringVector;
	}

	// Delete test data if it exists

	std::wstring unassignAllDevices = L"UPDATE [Devices] SET Currently_Issued_to = NULL WHERE Device_Name = 'TEST123'";
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	std::wstring removeEmpQuery = L"DELETE FROM [Employees] WHERE Employee_Name = 'Test Person'";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)removeEmpQuery.c_str(), SQL_NTS);

	std::wstring unassignDeviceQuery = L"DELETE FROM [Devices] WHERE Device_Name = 'TEST123'";
	SQLRETURN deleteResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignDeviceQuery.c_str(), SQL_NTS);


	// fail to create a test employee and make sure it doesnt work
	// create a test employee and check for it. Make sure the size of the data is correct
	
	int retcode;

	retcode = addEmployee(hStmt, L"Test Person", L"Tester", L"0", L"testp@gmail.com", L"555-555-5555", L"9999");	//Invalid location
	retcodeStringVector.push_back(testResult(retcode, -1) + L": addEmployee() catches invalid location");

	retcode = addEmployee(hStmt, L"Test Person", L"Tester", L"1", L"testp@gmail.com", L"555-555-5555", L"9999");	// Valid entry
	retcodeStringVector.push_back(testResult(retcode, 0) + L": addEmployee()");

	// fail to create a test device and make sure it doesn't work
	// create a test device and check for it. Make sure the size of the data is correct
	retcode = newDevice(hStmt, L"TEST123", L"AAAAAAAA", L"9999", L"11/11/11", L"1000", L"Win 11", isTestServer);	// Invalid Device Model
	retcodeStringVector.push_back(testResult(retcode, -1) + L": newDevice() catches invalid device model");

	retcode = newDevice(hStmt, L"TEST123", L"AAAAAAAA", L"1", L"11/11/11", L"1000", L"Win 11", isTestServer);	// Valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": newDevice()");

	// assign the test device to the test employee incorrectly, make sure it doesn't work
	// assign the test device to the employee correctly, make sure it dows work
	// check that the connection has been made

	retcode = assignDevice(hStmt, L"TEST123", L"aaaaaaaaaaaaaaaaaaa@gmail.com", L"11/11/11", L"1"); // Invalid Email
	retcodeStringVector.push_back(testResult(retcode, -1) + L": assignDevice() catches invalid email entry");

	retcode = assignDevice(hStmt, L"TEST123", L"testp@nhcares.com", L"11/11/11", L"1");	// Valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": assignDevice()");

	// Unassign device

	retcode = unassignDevice(hStmt, L"----------"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": unassignDevice() catches invalid device name");

	retcode = unassignDevice(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": unassignDevice()");

	// Delete the device
	retcode = removeDevice(hStmt, L"+++++++++++"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeDevice() catches invalid device name");

	retcode = removeDevice(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeDevice()");

	// Delete Employee
	retcode = removeEmployee(hStmt, getIdFromEmail(hStmt, L"******************")); // invalid employee name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeEmployee() catches invalid employee name");

	// Delete Employee
	retcode = removeEmployee(hStmt, getIdFromEmail(hStmt, L"testp@gmail.com")); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeEmployee()");

	// This should implicitly test all of the function implicitly
	// Next, write tests for each function explicitly

	return retcodeStringVector;
}

std::wstring testResult(int actualResult, int expectedResult)
{
	if (actualResult == expectedResult)
	{
		return L"SUCCESS";
	}

	else
	{
		return L"FAILED";
	}
}