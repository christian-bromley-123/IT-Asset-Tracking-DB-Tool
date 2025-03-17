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


#define SQL_QUERY_SIZE      1000 // Max. Num characters for SQL Query passed in.

// Connection
SQLHENV     hEnv = NULL;			// Environment handle
SQLHDBC     hDbc = NULL;			// Connection handle
SQLHSTMT    hStmt = NULL;			// Statement handle

bool isTestServer;

//Variables for Device
std::wstring deviceNumber;
std::wstring serialTag;
std::wstring deviceModelId;
std::wstring currentlyIssuedTo;
std::wstring purchaseDate;
std::wstring deviceCost;
std::wstring issueDate;
std::wstring firstUser;
std::wstring operatingSystem;


//Variables for employees
std::wstring employeeName;
std::wstring employeeTitle;
std::wstring employeeLocation;
std::wstring employeeEmailAddress;
std::wstring employeePhoneNumber;
std::wstring employeeExtension;
std::wstring employeeID;

//Loop breakers
char yesOrNo;
bool endUnassignLoop;
int checkVar;

//Functions
std::wstring getConString(char dbChoice);
void newDevice(SQLHSTMT hStmt);
void assignDevice(SQLHSTMT hStmt);
void unassignDevice(SQLHSTMT hStmt);
void addEmployee(SQLHSTMT hStmt);
void removeEmployee(SQLHSTMT hStmt);
void removeDevice(SQLHSTMT hStmt);
void writeLastDevice(std::wstring nhwsNumber);
void readLastDevice();
void enterKey();
int connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt);
SQLRETURN diagSQLError(int sqlHandle, SQLHANDLE handle);
std::wstring getIdFromEmail(SQLHANDLE handle, std::wstring employeeEmail);
std::wstring getLocationFromID(SQLHANDLE handle, std::wstring locationID);
std::wstring getResult(SQLHANDLE handle, int column);

