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

// main.hpp : Include file for standard system include files,
// or project specific include files.

#pragma once


#include <string>
#include <vector>

#include <windows.h>
#include <sql.h>
#include <sqlext.h>


#define         SQL_QUERY_SIZE      1000 // Max. Num characters for SQL Query passed in.

std::wstring                getConString(char dbChoice, bool &isTestServer);
int                         assignComputer(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser);
int                         assignPeripheral(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser);
int                         assignHotspot(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate);
int                         assignEquipment(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate);
int                         newComputer(SQLHSTMT hStmt, std::wstring computerName, std::wstring serialNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, std::wstring operatingSystem, bool isTestServer);
int                         newPeripheral(SQLHSTMT hStmt, std::wstring perhiperalName, std::wstring serialNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, bool isTestServer);
int                         newHotspot(SQLHSTMT hStmt, std::wstring phoneNumber, std::wstring imeiNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, bool isTestServer);
int                         newEquipment(SQLHSTMT hStmt, std::wstring peripheralName, std::wstring serialNumber, std::wstring deviceModelId, std::wstring datePurchased, std::wstring cost, bool isTestServer);
int                         unassignComputer(SQLHSTMT hStmt, std::wstring deviceNumber);
int                         unassignPeripheral(SQLHSTMT hStmt, std::wstring deviceNumber);
int                         unassignHotspot(SQLHSTMT hStmt, std::wstring deviceNumber);
int                         unassignEquipment(SQLHSTMT hStmt, std::wstring deviceNumber);
int                         addEmployee(SQLHSTMT hStmt, std::wstring employeeName, std::wstring employeeTitle, std::wstring employeeLocationId, std::wstring employeeEmailAddress, std::wstring employeePhoneNumber, std::wstring employeeExtension);
int                         removeEmployee(SQLHSTMT hStmt, std::wstring employeeID);
int                         removeComputer(SQLHSTMT hStmt, std::wstring computerName);
int                         removePeripheral(SQLHSTMT hStmt, std::wstring peripheralName);
int                         removeHotspot(SQLHSTMT hStmt, std::wstring hotspotName);
int                         removeEquipment(SQLHSTMT hStmt, std::wstring equipmentName);
int                         writeReport(SQLHANDLE hStmt, int option);
void                        writeLastDevice(std::wstring deviceNumber);
void                        readLastDevice();
void                        enterKey();
int                         connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt, char dbChoice, bool& isTestServer);
SQLRETURN                   diagSQLError(int sqlHandle, SQLHANDLE handle);
std::wstring                getIdFromEmail(SQLHANDLE handle, std::wstring employeeEmail);
std::wstring                getEmployeeNameFromID(SQLHANDLE hStmt, std::wstring employeeID);
std::wstring                getLocationFromID(SQLHANDLE handle, std::wstring locationID);
std::wstring                getLocationIDFromName(SQLHANDLE hStmt, std::wstring locationName);
std::wstring                getModelIdFromDeviceName(SQLHANDLE hStmt, std::wstring deviceName);
std::wstring                getModelIdFromModelName(SQLHANDLE hStmt, std::wstring modelName);
std::wstring                getResult(SQLHANDLE hStmt, int column = 1, int row = 1, bool lastResult = true);
std::vector<std::wstring>   getResultRow(SQLHANDLE hStmt, int row = 1, bool lastResult = true);
std::wstring                getResultColumnName(SQLHANDLE hStmt, int column);
std::vector<std::wstring>   getAllResultColumnNames(SQLHANDLE hStmt);
std::vector<std::wstring>   getColumn(SQLHANDLE hStmt, bool isDistinct, std::wstring table, std::wstring column, std::wstring param = L"", std::wstring target = L"");
std::wstring                getDeviceTypeFromModelId(SQLHANDLE hStmt, std::wstring deviceModelId);
std::wstring                getDeviceTableFromModelId(SQLHANDLE hStmt, std::wstring deviceModelId);
int                         recordDeviceTransaction(SQLHANDLE hStmt, std::wstring deviceName, std::wstring transactionType, std::wstring employee = L"");
int                         recordEmployeeTransaction(SQLHANDLE hStmt, std::wstring transactionType, std::wstring employee);
bool                        checkValid(SQLHANDLE handle, std::wstring table, std::wstring column, std::wstring param, std::wstring target);
bool                        isValidDate(std::wstring date);