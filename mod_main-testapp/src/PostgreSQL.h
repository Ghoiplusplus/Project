#include <pqxx/pqxx>
#include <iostream>
#include <windows.h>
void PostgresInit()
{
    string PasswordPostgreSQL = "xxxx"; 
    string dbName = "db_module";

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    while (true)
    {
        cout << "Введите пароль";
        cin >> PasswordPostgreSQL;
        string connInfo = "host=127.0.0.1 dbname=postgres user=postgres password=" + PasswordPostgreSQL;

        try
        {
            pqxx::connection conn(connInfo);

            if (conn.is_open())
            {
                cout << "Подключение к базе данных успешно установлено!" << endl;
                conn.close();
                break; 
            }
            else
            {
                cerr << "Не удалось подключиться к базе данных." << endl;
            }
        }
        catch (const pqxx::sql_error &e)
        {
            cerr << "Ошибка SQL: " << e.what() << endl;
            cerr << "Запрос: " << e.query() << endl;
        }
        catch (const exception &e)
        {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    try {
        pqxx::connection conn("host=127.0.0.1 user=postgres password=" + PasswordPostgreSQL);
        pqxx::work txn(conn);
        pqxx::result result = txn.exec("SELECT 1 FROM pg_database WHERE datname = '" + dbName + "'");
        if (!result.empty()) {
            cout << "База данных 'db_module' существует." << endl;
        } else {
            cout << "База данных 'db_module' не существует. Создание..." << endl;
            txn.commit();
            pqxx::connection conn_create("host=127.0.0.1 user=postgres password=" + PasswordPostgreSQL);
            pqxx::nontransaction nontxn(conn_create);
            nontxn.exec("CREATE DATABASE " + dbName);
            cout << "База данных 'db_module' успешно создана." << endl;
        }
    } catch (const exception &e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
    try {
        pqxx::connection conn("host=127.0.0.1 dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL);
        pqxx::work txn(conn);
        string tables[] = {"users", "disciplines", "tests", "questions", "attempts", "answers"};
        for (const auto& table : tables) {
            pqxx::result result = txn.exec(
                "SELECT 1 FROM information_schema.tables WHERE table_schema = 'public' AND table_name = '" + table + "'"
            );
            if (!result.empty()) {
                cout << "Таблица '" << table << "' существует." << endl;
            } else {
                cout << "Таблица '" << table << "' не существует. Создание..." << endl;
                if (table == "users") {
                    txn.exec(
                        "CREATE TABLE users ("
                        "id SERIAL PRIMARY KEY, "           // ID пользователя
                        "last_name VARCHAR(50) NOT NULL, "  // Фамилия
                        "first_name VARCHAR(50) NOT NULL, " // Имя
                        "middle_name VARCHAR(50), "         // Отчество
                        "roles TEXT[], "                    // Список ролей (массив)
                        "banned BOOLEAN DEFAULT FALSE)"     // Бан
                    );
                } else if (table == "disciplines") {
                    txn.exec(
                        "CREATE TABLE disciplines ("
                        "id SERIAL PRIMARY KEY, "               // ID дисциплины
                        "name VARCHAR(100) NOT NULL UNIQUE, "   // Название дисциплины
                        "description TEXT, "                    // Описание
                        "teacher_id INT REFERENCES users(id), " // ID преподавателя
                        "student_ids INT[])"                    // Список ID студентов (массив)
                    );
                } else if (table == "tests") {
                    txn.exec(
                        "CREATE TABLE tests ("
                        "id SERIAL PRIMARY KEY, "                           // ID теста
                        "discipline_id INT REFERENCES disciplines(id), "    // ID дисциплины
                        "author_id INT REFERENCES users(id), "              // ID автора
                        "question_ids INT[], "                              // Список ID вопросов (массив)
                        "attempt_ids INT[], "                               // Список ID попыток (массив)
                        "is_active BOOLEAN DEFAULT TRUE)"                   // Активен ли тест
                    );
                } else if (table == "questions") {
                    txn.exec(
                        "CREATE TABLE questions ("
                        "id SERIAL PRIMARY KEY, "               // ID вопрос
                        "author_id INT REFERENCES users(id), "  // ID автора
                        "version INT NOT NULL, "                // Версия вопроса
                        "title VARCHAR(255) NOT NULL, "         // Название вопроса
                        "description TEXT, "                    // Описание вопроса
                        "options TEXT[], "                      // Список вариантов ответов (массив)
                        "correct_option_index INT)"             // Номер правильного ответа
                    );
                } else if (table == "attempts") {
                    txn.exec(
                        "CREATE TABLE attempts ("
                        "id SERIAL PRIMARY KEY, "                           // ID попытки
                        "user_id INT REFERENCES users(id), "                // ID пользователя
                        "test_id INT REFERENCES tests(id), "                // ID теста
                        "discipline_id INT REFERENCES disciplines(id), "    // ID дисциплины
                        "answer_ids INT[], "                                // Список ID ответов (массив)
                        "is_completed BOOLEAN DEFAULT FALSE)"               // Завершена ли попытка
                    );
                } else if (table == "answers") {
                    txn.exec(
                        "CREATE TABLE answers ("
                        "id SERIAL PRIMARY KEY, "                       // ID ответа
                        "attempt_id INT REFERENCES attempts(id), "      // ID попытки
                        "question_id INT REFERENCES questions(id), "    // ID вопроса
                        "answer_index INT)"                             // Индекс ответа
                    );
                }
                cout << "Таблица '" << table << "' успешно создана." << endl;
            }
        }
        txn.commit();
    } catch (const exception &e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    cout << endl;
}

