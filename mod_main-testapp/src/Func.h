
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <iomanip>
#include <winsock2.h>
#include <string>
#include <iostream>

using namespace std;
inline void beautyPrint(SOCKET soc, string text)
{
    cout << '[' << setfill(' ') << setw(7) << soc << "] " << text << endl;
}