#include <pqxx/pqxx>
#include <windows.h> 
#include <vector>
#include <string>
#include <iostream>
using namespace std;

vector<int> sql_get_array_int(const string& column_name, const string& table_name, int id);
vector<string> sql_get_array_str(const string& column_name, const string& table_name, int id);

void sql_del_from_array_int(const string& column_name, const string& table_name, int id, int num);
void sql_add_to_array_int(const string& column_name, const string& table_name, int id, int num);

vector<string> sql_get_list_str (const string& column_name, const string& table_name);
vector<int> sql_get_list_int (const string& column_name, const string& table_name);

string sql_get_one_str(const string& column_name, const string& table_name, int id);
int sql_get_one_int(const string& column_name, const string& table_name, int id);
bool sql_get_one_bool(const string& column_name, const string& table_name, int id);

void sql_update_one_str(const string& column_name, const string& table_name, int id, const string new_value);
void sql_update_one_bool(const string& column_name, const string& table_name, int id, bool new_value);

void PostgresInit();
void add_user();

string get_db_name();
string get_db_password();