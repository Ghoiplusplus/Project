
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <string>
#include <chrono>
#include "Func.h"
#include <winsock2.h>
#include <iostream>

enum class Resource {
    Users,
    Discipline,
    Questions,
    Tests,
    Attempt,
    Answers,
    Error
};
Resource CheckToken(SOCKET clientSocket, char* message, int msgSize);
