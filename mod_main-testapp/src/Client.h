#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>

#include "Token.h"

#include "Func.h"

void handleClient(SOCKET clientSocket);
