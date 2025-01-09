#include "Token.h"
using namespace std;

Resource CheckToken(SOCKET clientSocket, char* message, int msgSize)
{
    string secret = "Ключ"; // Заранее определённый токен
    string receivedToken(message, msgSize); // Полученный токен из сообщения

    beautyPrint(clientSocket, "Поиск токена");

    // Проверка токена
    if (receivedToken == secret) {
        beautyPrint(clientSocket, "Токен найден");
        return Resource::Success; // Возвращаем успешный результат
    } else {
        beautyPrint(clientSocket, "Токен не найден");
        return Resource::Error; // Возвращаем ошибку
    }
}
