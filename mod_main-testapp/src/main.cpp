
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <thread>
#include <iostream>
#include "Client.h"	
#include "getAddress.h"	
#include <vector>
#include <string>
const char* ADRES = "127.0.0.1";
#define PORT  1111
using namespace std;

int main()
{
	cout << "Load lib...       ";
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "Error.\n";
		return 1;
	}
	else {
		cout << "Done.\n";
	}
	cout << "Create socket...  ";
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error.\n";
		WSACleanup();
		return 1;
	}
	else {
		cout << "Done.\n";
	}
	cout << "Adres and port... ";

	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ADRES);
	serverAddr.sin_port = htons(PORT);
	bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	cout << "Done.\n";
	listen(serverSocket, SOMAXCONN);

	cout << "\nServer started" << endl << endl;
	cout << "Address - " << ADRES << endl;
	cout << "Port    - " << PORT << endl;
	cout << "Link    - " << "http://" << ADRES << ':' << PORT << '/' << endl << endl;

	vector<thread> clientThreads;

	while (true)
	{
		sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);
		SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == INVALID_SOCKET) {
			cerr << "Accept failed: " << WSAGetLastError() << endl;
			continue;
		}
		clientThreads.emplace_back(thread(handleClient, clientSocket));
	}
	cout << "test";
	return 0;
}

