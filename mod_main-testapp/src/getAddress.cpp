
#include "getAddress.h"
#include <iostream>
#include <vector>
#include <thread>
using namespace std;
const char* getLocalIPAddress() {
	const char* address = "127.0.0.1";
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		return address;
	}
	cout << endl;
	cout << "Hostname: " << hostname << endl;	
	struct hostent* hostInfo = gethostbyname(hostname);
	if (hostInfo == nullptr) {
		return address;
	}
	cout << "IP-адрес:" << endl;
	for (int i = 0; hostInfo->h_addr_list[i] != nullptr; i++) {
		struct in_addr addr;
		memcpy(&addr, hostInfo->h_addr_list[i], sizeof(struct in_addr));
		cout << "- " << inet_ntoa(addr) << endl;
		address = inet_ntoa(addr);
	}
	cout << endl;
	return address;
}