#include "PostgreSQL.h"
using namespace std;

extern string PasswordPostgreSQL = "1234";
extern string dbName = "db_module";

string get_db_name()
{
    return dbName;
}

string get_db_password()
{
    return PasswordPostgreSQL;
}


void PostgresInit()
{
    
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    while (true)
    {
        cout << "Введите пароль для PostgreSQL: ";
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
                        "disciplines INT[], "               // Список дисциплин
                        "banned BOOLEAN DEFAULT FALSE)"     // Бан
                    );
                } else if (table == "disciplines") {
                    txn.exec(
                        "CREATE TABLE disciplines ("
                        "id SERIAL PRIMARY KEY, "               // ID дисциплины
                        "name VARCHAR(100) NOT NULL UNIQUE, "   // Название дисциплины
                        "description TEXT, "                    // Описание
                        "teacher_id INT REFERENCES users(id))"  // ID преподавателя
                    );
                } else if (table == "tests") {
                    txn.exec(
                        "CREATE TABLE tests ("
                        "id SERIAL PRIMARY KEY, "                           // ID теста
                        "discipline_id INT REFERENCES disciplines(id), "    // ID дисциплины
                        "author_id INT REFERENCES users(id), "              // ID автора
                        "name TEXT NOT NULL, "                              // Название теста
                        "question_ids INT[], "                              // Список ID вопросов (массив)
                        "attempt_ids INT[], "                               // Список ID попыток (массив)
                        "is_active BOOLEAN DEFAULT TRUE, "                  // Активен ли тест
                        "deleted BOOLEAN DEFAULT FALSE)"                    // Флаг удаления теста
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
                        "correct_option_index INT, "            // Номер правильного ответа
                        "deleted BOOLEAN DEFAULT FALSE)"        // Флаг удаления вопроса
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



void add_user() {
    
    string last_name = "user_lastName2";
    string first_name = "user_firstName2";
    string middle_name = "user_middleName2";
    vector<string> roles;
    for (int i = 0; i < 5; i++)
        roles.push_back("role" + to_string(i));
    bool banned = false;
    string password = PasswordPostgreSQL;

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    try {
        pqxx::connection conn("host=127.0.0.1 dbname=" + dbName + " user=postgres password=" + password);

        pqxx::work txn(conn);


        string roles_array = "{";
        for (size_t i = 0; i < roles.size(); ++i) {
            roles_array += "\"" + txn.esc(roles[i]) + "\""; 
            if (i < roles.size() - 1) {
                roles_array += ", ";
            }
        }
        roles_array += "}";
        string query = "INSERT INTO users (last_name, first_name, middle_name, roles, banned) "
                            "VALUES ('" + txn.esc(last_name) + "', '" + txn.esc(first_name) + "', '" + txn.esc(middle_name) + "', "
                            "'" + roles_array + "', " + (banned ? "TRUE" : "FALSE") + ")";

        txn.exec(query);

        txn.commit();

        cout << "Пользователь успешно добавлен." << endl;
    } catch (const exception &e) {
        cerr << "Ошибка при добавлении пользователя: " << e.what() << endl;
    }
}





vector<int> sql_get_array_int (const string& column_name, const string& table_name, int id)
{
    vector<int> arr;

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            string query = "SELECT " + column_name + " FROM " + table_name + " WHERE id = " + txn.quote(id);
            pqxx::result result = txn.exec(query);

            if (!result.empty()) {
                pqxx::array_parser parser = result[0][0].as_array();
                pair<pqxx::array_parser::juncture, string> elem;

                while ((elem = parser.get_next()).first != pqxx::array_parser::juncture::done) {
                    if (elem.first == pqxx::array_parser::juncture::string_value) {
                        arr.push_back(stoi(elem.second));
                    }
                }
            } else {
                cout << "   Error." << endl;
            }

            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return arr;
}




vector<string> sql_get_array_str(const string& column_name, const string& table_name, int id)
{
    vector<string> arr;

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            string query = "SELECT " + column_name + " FROM " + table_name + " WHERE id = " + txn.quote(id);
            pqxx::result result = txn.exec(query);

            if (!result.empty()) {
                pqxx::array_parser parser = result[0][0].as_array();
                pair<pqxx::array_parser::juncture, string> elem;

                while ((elem = parser.get_next()).first != pqxx::array_parser::juncture::done) {
                    if (elem.first == pqxx::array_parser::juncture::string_value) {
                        arr.push_back(elem.second);
                    }
                }
            } else {
                cout << "   Error." << endl;
            }

            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return arr;
}






vector<string> sql_get_list_str (const string& column_name, const string& table_name)
{
    vector<string> list;

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            pqxx::result result = txn.exec("SELECT " + column_name + " FROM " + table_name);

            for (auto row : result) {
                list.push_back(row[0].as<string>());
            }
            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return list;
}



vector<int> sql_get_list_int (const string& column_name, const string& table_name)
{
    vector<int> list;

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            pqxx::result result = txn.exec("SELECT " + column_name + " FROM " + table_name);

            for (auto row : result) {
                list.push_back(row[0].as<int>());
            }
            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return list;
}


string sql_get_one_str(const string& column_name, const string& table_name, int id)
{
    string one = "";

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            pqxx::result result = txn.exec("SELECT " + column_name + " FROM " + table_name + " WHERE id = " + to_string(id));

            auto row = result[0];
            one = row[0].as<string>();
            
            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return one;
}

int sql_get_one_int(const string& column_name, const string& table_name, int id)
{
    int one = -3;

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            pqxx::result result = txn.exec("SELECT " + column_name + " FROM " + table_name + " WHERE id = " + to_string(id));

            auto row = result[0];
            one = row[0].as<int>();
            
            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return one;
}

bool sql_get_one_bool(const string& column_name, const string& table_name, int id)
{
    bool one = false;

    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);       //  начало транзакции
            pqxx::result result = txn.exec("SELECT " + column_name + " FROM " + table_name + " WHERE id = " + to_string(id));

            auto row = result[0];
            one = row[0].as<bool>();
            
            txn.commit();               //  конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }

    return one;
}


void sql_update_one_str(const string& column_name, const string& table_name, int id, const string new_value)
{
    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);  // Начало транзакции
            string query = 
                "UPDATE " + table_name + " SET " + column_name + " = " + txn.quote(new_value) + " WHERE id = " + txn.quote(id);
            txn.exec(query);
            txn.commit();           // конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }
}


void sql_update_one_bool(const string& column_name, const string& table_name, int id, bool new_value)
{
    try {
        // Подключение к базе данных
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn);  // Начало транзакции
            string query = 
                "UPDATE " + table_name + " SET " + column_name + " = " + txn.quote(new_value) + " WHERE id = " + txn.quote(id);
            txn.exec(query);
            txn.commit();           // конец транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }
}




void sql_del_from_array_int(const string& column_name, const string& table_name, int id, int num) {
    try {
        // Подключение к базе данных
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");

        if (conn.is_open()) {
            pqxx::work txn(conn); // Начало транзакции

            // Формируем SQL-запрос для удаления элемента из массива
            string query = "UPDATE " + table_name + " SET " + column_name + 
            " = array_remove(" + column_name + ", " + txn.quote(num) + ") WHERE id = " + txn.quote(id);
            
            // Выполняем запрос
            txn.exec(query);

            txn.commit(); // Завершение транзакции
        } else {
            cerr << "   Failed to connect to database." << endl;
        }
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }
}



void sql_add_to_array_int(const string& column_name, const string& table_name, int id, int num)
{
    try {
        pqxx::connection conn("dbname=" + dbName + " user=postgres password=" + PasswordPostgreSQL + " host=127.0.0.1 port=5432");
        pqxx::work txn(conn);

        txn.exec_params(
            "UPDATE " + table_name +
            " SET " + column_name + " = array_append(" + column_name + ", $1) "
            "WHERE id = $2",
            num, id
        );
        txn.commit();
    } catch (const exception &e) {
        cerr << "   Error: " << e.what() << endl;
    }
}