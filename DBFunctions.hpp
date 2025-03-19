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

// Connection
extern SQLHENV         hEnv;			// Environment handle
extern SQLHDBC         hDbc;			// Connection handle
extern SQLHSTMT        hStmt;			// Statement handle

extern bool            isTestServer;

//Variables for Device
extern std::wstring    deviceNumber;
extern std::wstring    serialTag;
extern std::wstring    deviceModelId;
extern std::wstring    currentlyIssuedTo;
extern std::wstring    purchaseDate;
extern std::wstring    deviceCost;
extern std::wstring    issueDate;
extern std::wstring    firstUser;
extern std::wstring    operatingSystem;


//Variables for employees
extern std::wstring    employeeName;
extern std::wstring    employeeTitle;
extern std::wstring    employeeLocation;
extern std::wstring    employeeEmailAddress;
extern std::wstring    employeePhoneNumber;
extern std::wstring    employeeExtension;
extern std::wstring    employeeID;

//Loop breakers
extern char            yesOrNo;
extern bool            endUnassignLoop;
extern int             checkVar;

//Functions
std::wstring    getConString(char dbChoice);
void            newDevice(SQLHSTMT hStmt);
void            assignDevice(SQLHSTMT hStmt);
void            unassignDevice(SQLHSTMT hStmt);
void            addEmployee(SQLHSTMT hStmt);
void            removeEmployee(SQLHSTMT hStmt);
void            removeDevice(SQLHSTMT hStmt);
void            writeLastDevice(std::wstring nhwsNumber);
void            readLastDevice();
void            enterKey();
int             connectDatabase(SQLHENV& hEnv, SQLHDBC& hDbc, SQLHSTMT& hStmt);
SQLRETURN       diagSQLError(int sqlHandle, SQLHANDLE handle);
std::wstring    getIdFromEmail(SQLHANDLE handle, std::wstring employeeEmail);
std::wstring    getLocationFromID(SQLHANDLE handle, std::wstring locationID);
std::wstring    getResult(SQLHANDLE handle, int column);

