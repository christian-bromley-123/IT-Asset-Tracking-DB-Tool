#include "..\include\tests.hpp"
#include "..\include\DBFunctions.hpp"

#include <iostream>
#include <string>




std::vector<std::wstring> testAll(SQLHANDLE hStmt, bool isTestServer)
{
	/* 
	
	Test all of the Create and Delete functions.
	
	*/
	std::vector<std::wstring>retcodeStringVector;
	// Make sure the test server is selected
	if (!isTestServer)
	{
		std::vector<std::wstring> retcodeStringVector;
		return retcodeStringVector;
	}

	
	// Clear the DB of all potential test entries 

	clearTestData(hStmt);

	// fail to create a test employee and make sure it doesnt work
	// create a test employee and check for it. Make sure the size of the data is correct
	
	int retcode;

	retcode = addEmployee(hStmt, L"Test Person", L"Tester", L"0", L"testp@gmail.com", L"555-555-5555", L"9999");	//Invalid location
	retcodeStringVector.push_back(testResult(retcode, -1) + L": addEmployee() catches invalid location");

	retcode = addEmployee(hStmt, L"Test Person", L"Tester", L"1", L"testp@gmail.com", L"555-555-5555", L"9999");	// Valid entry
	retcodeStringVector.push_back(testResult(retcode, 0) + L": addEmployee()");

	// Create, assign, unassign, and delete a computer
	retcode = newComputer(hStmt, L"TEST123", L"AAAAAAAA", L"****", L"11/11/11", L"1000", L"Win 11", true);	// Invalid Device Model
	retcodeStringVector.push_back(testResult(retcode, -1) + L": newComputer() catches invalid device model");

	retcode = newComputer(hStmt, L"TEST123", L"AAAAAAAA", L"1", L"11/11/11", L"1000", L"Win 11", true);	// Valid if device model corresponds to a computer
	retcodeStringVector.push_back(testResult(retcode, 0) + L": newComputer()");

	retcode = assignComputer(hStmt, L"TEST123", L"aaaaaaaaaaaaaaaaaaa@gmail.com", L"11/11/11", L"1"); // Invalid Email
	retcodeStringVector.push_back(testResult(retcode, -1) + L": assignComputer() catches invalid email entry");

	retcode = assignComputer(hStmt, L"TEST123", L"testp@gmail.com", L"11/11/11", L"1");	// Valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": assignComputer()");

	retcode = unassignComputer(hStmt, L"----------"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": unassignComputer() catches invalid device name");

	retcode = unassignComputer(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": unassignComputer()");

	retcode = removeComputer(hStmt, L"+++++++++++"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeComputer() catches invalid device name");

	retcode = removeComputer(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeComputer()");

	// Create, assign, unassign, and delete a peripheral
	retcode = newPeripheral(hStmt, L"TEST123", L"AAAAAAAA", L"****", L"11/11/11", L"1000", true);	// Invalid Device Model
	retcodeStringVector.push_back(testResult(retcode, -1) + L": newPeripheral() catches invalid device model");

	retcode = newPeripheral(hStmt, L"TEST123", L"AAAAAAAA", L"27", L"11/11/11", L"1000", true);	// Valid if device model corresponds to a peripheral
	retcodeStringVector.push_back(testResult(retcode, 0) + L": newPeripheral()");

	retcode = assignPeripheral(hStmt, L"TEST123", L"aaaaaaaaaaaaaaaaaaa@gmail.com", L"11/11/11", L"1"); // Invalid Email
	retcodeStringVector.push_back(testResult(retcode, -1) + L": assignComputer() catches invalid email entry");

	retcode = assignPeripheral(hStmt, L"TEST123", L"testp@gmail.com", L"11/11/11", L"1");	// Valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": assignComputer()");

	retcode = unassignPeripheral(hStmt, L"----------"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": unassignComputer() catches invalid device name");

	retcode = unassignPeripheral(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": unassignComputer()");

	retcode = removePeripheral(hStmt, L"+++++++++++"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeComputer() catches invalid device name");

	retcode = removePeripheral(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeComputer()");

	// Create, assign, unassign, and delete a hotspot
	retcode = newHotspot(hStmt, L"TEST123", L"AAAAAAAA", L"****", L"11/11/11", L"1000", true);	// Invalid Device Model
	retcodeStringVector.push_back(testResult(retcode, -1) + L": newHotspot() catches invalid device model");

	retcode = newHotspot(hStmt, L"TEST123", L"AAAAAAAA", L"30", L"11/11/11", L"1000", true);	// Valid if device model corresponds to a hotspot
	retcodeStringVector.push_back(testResult(retcode, 0) + L": newHotspot()");

	retcode = assignHotspot(hStmt, L"TEST123", L"aaaaaaaaaaaaaaaaaaa@gmail.com", L"11/11/11"); // Invalid Email
	retcodeStringVector.push_back(testResult(retcode, -1) + L": assignComputer() catches invalid email entry");

	retcode = assignHotspot(hStmt, L"TEST123", L"testp@gmail.com", L"11/11/11");	// Valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": assignComputer()");

	retcode = unassignHotspot(hStmt, L"----------"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": unassignComputer() catches invalid device name");

	retcode = unassignHotspot(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": unassignComputer()");

	retcode = removeHotspot(hStmt, L"+++++++++++"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeComputer() catches invalid device name");

	retcode = removeHotspot(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeComputer()");


	// Create, assign, unassign, and delete office equipment
	retcode = newEquipment(hStmt, L"TEST123", L"AAAAAAAA", L"****", L"11/11/11", L"1000", true);	// Invalid Device Model
	retcodeStringVector.push_back(testResult(retcode, -1) + L": newEquipment() catches invalid device model");
	
	retcode = newEquipment(hStmt, L"TEST123", L"AAAAAAAA", L"24", L"11/11/11", L"1000", true);	// Valid if device model corresponds to office equipment
	retcodeStringVector.push_back(testResult(retcode, 0) + L": newEquipment()");

	retcode = assignEquipment(hStmt, L"TEST123", L"aaaaaaaaaaaaaaaaaaa@gmail.com", L"11/11/11"); // Invalid Email
	retcodeStringVector.push_back(testResult(retcode, -1) + L": assignComputer() catches invalid email entry");

	retcode = assignEquipment(hStmt, L"TEST123", L"testp@gmail.com", L"11/11/11");	// Valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": assignComputer()");

	retcode = unassignEquipment(hStmt, L"----------"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": unassignComputer() catches invalid device name");

	retcode = unassignEquipment(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": unassignComputer()");

	retcode = removeEquipment(hStmt, L"+++++++++++"); // invalid name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeComputer() catches invalid device name");

	retcode = removeEquipment(hStmt, L"TEST123"); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeComputer()");

	// Delete Employee
	retcode = removeEmployee(hStmt, getIdFromEmail(hStmt, L"******************")); // invalid employee name
	retcodeStringVector.push_back(testResult(retcode, -1) + L": removeEmployee() catches invalid employee name");

	// Delete Employee
	retcode = removeEmployee(hStmt, getIdFromEmail(hStmt, L"testp@gmail.com")); // valid
	retcodeStringVector.push_back(testResult(retcode, 0) + L": removeEmployee()");

	// This should implicitly test all of the functions
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

void clearTestData(SQLHANDLE hStmt)
{
	/* 
		Clears any leftover data from failed tests
	*/


	// Unassign test data from every device table
	std::wstring unassignAllDevices = L"UPDATE [Computers] SET Currently_Issued_to = NULL WHERE Computer_Name = 'TEST123'";
	SQLRETURN statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	unassignAllDevices = L"UPDATE [Peripherals] SET Currently_Issued_to = NULL WHERE Peripheral_Name = 'TEST123'";
	statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	unassignAllDevices = L"UPDATE [Hotspots] SET Currently_Issued_to = NULL WHERE Phone_Number = '555-555-5555'";
	statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	unassignAllDevices = L"UPDATE [Office_Equipment] SET Currently_Issued_to = NULL WHERE Equipment_Name = 'TEST123'";
	statementResult;
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)unassignAllDevices.c_str(), SQL_NTS);

	// Delete Test employee
	std::wstring removeEmpQuery = L"DELETE FROM [Employees] WHERE Employee_Name = 'Test Person'";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)removeEmpQuery.c_str(), SQL_NTS);

	// Delete all test devices
	std::wstring deleteDeviceQuery = L"DELETE FROM [Computers] WHERE Computer_Name = 'TEST123'";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)deleteDeviceQuery.c_str(), SQL_NTS);

	deleteDeviceQuery = L"DELETE FROM [Peripherals] WHERE Peripheral_Name = 'TEST123'";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)deleteDeviceQuery.c_str(), SQL_NTS);

	deleteDeviceQuery = L"DELETE FROM [Hotspots] WHERE Phone_Number = '555-555-5555'";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)deleteDeviceQuery.c_str(), SQL_NTS);

	deleteDeviceQuery = L"DELETE FROM [Office_Equipment] WHERE Equipment_Name = 'TEST123'";
	statementResult = SQLExecDirect(hStmt, (SQLWCHAR*)deleteDeviceQuery.c_str(), SQL_NTS);

	return;
}

