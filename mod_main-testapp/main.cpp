#include <iostream>
#include <string>
#include <unordered_map>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <hiredis/hiredis.h>
#include <jwt-cpp/jwt.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class WebClient {
public:
    WebClient(const std::string& url) : listener(url) {
        listener.support(methods::GET, std::bind(&WebClient::handle_get, this, std::placeholders::_1));
        listener.support(methods::POST, std::bind(&WebClient::handle_post, this, std::placeholders::_1));
    }

    void open() {
        listener
            .open()
            .then([this]() { std::cout << "Starting to listen at: " << listener.uri().to_string() << std::endl; })
            .wait();
    }

private:
    http_listener listener;

    void handle_get(http_request request) {
        auto path = request.request_uri().path();
        auto session_token = get_session_token(request);

        if (session_token.empty()) {
            // Неизвестный пользователь
            handle_unknown_user(request);
        }
        else {
            // Проверка сессии в Redis
            if (check_session_in_redis(session_token)) {
                // Логика для авторизованного пользователя
                handle_authorized_user(request, session_token);
            }
            else {
                // Логика для анонимного пользователя
                handle_anonymous_user(request);
            }
        }
    }

    void handle_post(http_request request) {
        // Обработка POST запросов (например, для авторизации)
    }

    std::string get_session_token(const http_request& request) {
        // Извлечение токена сессии из куки
        return ""; // Заглушка
    }

    bool check_session_in_redis(const std::string& session_token) {
        // Проверка наличия токена сессии в Redis
        return false; // Заглушка
    }

    void handle_unknown_user(http_request request) {
        // Логика для обработки неизвестного пользователя
        json::value response_data;
        response_data[U("message")] = json::value::string(U("Пожалуйста, авторизуйтесь."));
        request.reply(status_codes::Unauthorized, response_data);
    }

    void handle_anonymous_user(http_request request) {
        // Логика для обработки анонимного пользователя
        json::value response_data;
        response_data[U("message")] = json::value::string(U("Вы анонимный пользователь."));
        request.reply(status_codes::OK, response_data);
    }

    void handle_authorized_user(http_request request, const std::string& session_token) {
        // Логика для обработки авторизованного пользователя
        json::value response_data;
        response_data[U("message")] = json::value::string(U("Добро пожаловать, авторизованный пользователь."));
        request.reply(status_codes::OK, response_data);
    }
};

int main() {
    WebClient client("http://localhost:8080");
    client.open();

    std::string line;
    std::getline(std::cin, line); // Ожидание ввода для завершения программы
    return 0;
}