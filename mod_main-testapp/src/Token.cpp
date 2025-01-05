

#include "Token.h"
using namespace std;

Resource CheckToken(SOCKET clientSocket, char* message, int msgSize)
{
    string secret = "Ключ";

    beautyPrint(clientSocket, "Поиск токена");


    beautyPrint(clientSocket, "Токен не найден");

    return Resource::Error;
}
