#include "Client.h"
using namespace std;
void handleClient(SOCKET clientSocket) {
    
    cout << "Новое подключение (Сокет) " << clientSocket << endl;
    const int msgSize = 1024;
    char* message = new char[msgSize];

    while (true)
    {
        int bytesReceived = recv(clientSocket, message, msgSize, NULL);

        if (bytesReceived == 0 || bytesReceived == -1) {
            beautyPrint(clientSocket, "Client has disconnected or a connection error has occurred.");
            break;
        }
        beautyPrint(clientSocket, "Message : " +  string(message));
    
        switch (CheckToken(clientSocket, message, msgSize))
        {
        case Resource::Users:
            beautyPrint(clientSocket, "Resource: Users");
            break;

        case Resource::Discipline:
            beautyPrint(clientSocket, "Resource: Discipline");
            break;

        case Resource::Questions:
            beautyPrint(clientSocket, "Resource: Questions");
            break;

        case Resource::Tests:
            beautyPrint(clientSocket, "Resource: Tests");
            break;

        case Resource::Attempt:
            beautyPrint(clientSocket, "Resource: Attempt");
            break;

        case Resource::Answers:
            beautyPrint(clientSocket, "Resource: Answers");
            break;
        
        case Resource::Error:
            beautyPrint(clientSocket, "Resource: Error");

            const char* httpResponse = 
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Access Denied";
    
            beautyPrint(clientSocket, "Send code 403");
            send(clientSocket, httpResponse, strlen(httpResponse), 0);

            closesocket(clientSocket);
            beautyPrint(clientSocket, "Socket closed.");
            break;
        }
    }    
}


