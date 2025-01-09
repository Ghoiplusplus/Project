#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>


enum class Resource {
    Success,   // Успешный результат
    Error,     // Ошибка
    Users,     // Ресурс: Пользователи
    Discipline, // Ресурс: Дисциплина
    Questions, // Ресурс: Вопросы
    Tests,     // Ресурс: Тесты
    Attempt,   // Ресурс: Попытка
    Answers    // Ресурс: Ответы
};


void beautyPrint(SOCKET clientSocket, const std::string& message);


void handleClient(SOCKET clientSocket);


Resource CheckToken(SOCKET clientSocket, char* message, int msgSize);
