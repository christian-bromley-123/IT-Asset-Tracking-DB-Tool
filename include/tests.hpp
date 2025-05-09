
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <wchar.h>

std::vector<std::wstring>	testAll(SQLHANDLE hStmt, bool isTestServer);
std::wstring				testResult(int actualResult, int expectedResult);
void						clearTestData(SQLHANDLE hStmt);
