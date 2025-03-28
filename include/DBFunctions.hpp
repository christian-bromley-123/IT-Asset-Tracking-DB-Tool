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

//Libraries
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <fstream>
#include <filesystem>

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#include <sal.h>


#define         SQL_QUERY_SIZE      1000 // Max. Num characters for SQL Query passed in.

std::wstring    getConString(char dbChoice, bool &isTestServer);
int             newDevice(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring serialTag, std::wstring deviceModelId, std::wstring purchaseDate, std::wstring deviceCost, std::wstring operatingSystem, bool isTestServer);
int             assignDevice(SQLHSTMT hStmt, std::wstring deviceNumber, std::wstring employeeEmailAddress, std::wstring issueDate, std::wstring firstUser);
int             unassignDevice(SQLHSTMT hStmt, std::wstring deviceNumber);
int             addEmployee(SQLHSTMT hStmt, std::wstring employeeName, std::wstring employeeTitle, std::wstring employeeLocation, std::wstring employeeEmailAddress, std::wstring employeePhoneNumber, std::wstring employeeExtension);
int             removeEmployee(SQLHSTMT hStmt, std::wstring employeeID);
int             removeDevice(SQLHSTMT hStmt, std::wstring deviceNumber);
void            writeLastDevice(std::wstring deviceNumber);
void            readLastDevice();
void            enterKey();
int             connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt, char dbChoice, bool& isTestServer);
SQLRETURN       diagSQLError(int sqlHandle, SQLHANDLE handle);
std::wstring    getIdFromEmail(SQLHANDLE handle, std::wstring employeeEmail);
std::wstring    getLocationFromID(SQLHANDLE handle, std::wstring locationID);
std::wstring    getResult(SQLHANDLE handle, int column);
bool            checkValid(SQLHANDLE handle, std::wstring table, std::wstring column, std::wstring param, std::wstring target);