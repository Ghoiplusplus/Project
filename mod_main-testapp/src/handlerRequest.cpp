
#include "handlerRequest.h"
using namespace std;


bool Unauthorized(httplib::Response& res, unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> permission)
{
    if (permission.empty()) {
        cout << "   Token not found or invalid. return status 401" << endl;
        res.status = 401; 
        res.set_content("Unauthorized: Token not found or invalid.", "text/plain");
        return true;
    }
    cout << "   processing..." << endl;
    return false;
}





void AddUser (const httplib::Request& req, httplib::Response& res) {
    if (req.body.empty()) {
        res.status = 400; // Bad Request
        res.set_content("Request body is empty", "text/plain");
        return;
    }

    picojson::Value jsonData;
    picojson::CharReaderBuilder reader;
    string errs;

    istringstream s(req.body);
    if (!picojson::parseFromStream(reader, s, &jsonData, &errs)) {
        res.status = 400; // Bad Request
        res.set_content("Invalid picojson: " + errs, "text/plain");
        return;
    }

    string last_name = jsonData["last_name"].asString();
    string first_name = jsonData["first_name"].asString();
    string middle_name = jsonData.get("middle_name", "").asString();
    vector<string> roles;
    for (const auto& role : jsonData["roles"]) {
        roles.push_back(role.asString());
    }
    bool banned = jsonData.get("banned", false).asBool();

    try {
        pqxx::connection conn("host=127.0.0.1 dbname=db_module user=postgres password=M6771350ax");
        pqxx::work txn(conn);

        txn.exec0("INSERT INTO users (last_name, first_name, middle_name, roles, banned) VALUES (" +
                  txn.quote(last_name) + ", " +
                  txn.quote(first_name) + ", " +
                  txn.quote(middle_name) + ", " +
                  txn.quote(roles) + ", " +
                  (banned ? "TRUE" : "FALSE") + ");");

        txn.commit();
        res.status = 201; // Created
        res.set_content("User  added successfully", "text/plain");
    } catch (const exception& e) {
        res.status = 500; // Internal Server Error
        res.set_content("Database error: " + string(e.what()), "text/plain");
    }
}
void GetUserList(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user-list:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson jsonRes;
    for (int i = 0; i < 10; i++)
        jsonRes["user" + to_string(i)] = i;
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void GetUserNamea(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user name:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int id = stoi(req.matches[1]);
    picojson::picojson jsonRes;
    jsonRes["first_name"] = "first_name" + to_string(id);      
    jsonRes["last_name"] = "last_name" + to_string(id);       
    jsonRes["middle_name"] = "middle_name" + to_string(id);

    res.set_content(jsonRes.dump(), "application/picojson");

    cout << "   End." << endl;
}

void SetUserName(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set user name:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    
    picojson::picojson body = picojson::picojson::parse(req.body);

    string first_name = body["first_name"];  
    string last_name = body["last_name"];      
    string middle_name = body["middle_name"]; 
    cout << "   New full name: " << first_name << ' ' << last_name << ' ' << middle_name << endl;
    cout << "   End." << endl;
}

void GetUserCourses(const httplib::Request& req, httplib::Response& res)
{
   
    cout << "Get user courses:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
        jsonRes.push_back("Disc" + to_string(i));

    res.set_content(jsonRes.dump(), "application/picojson");


    cout << "   End." << endl;
}

void GetUserGrades(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user grades:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
        jsonRes.push_back("5");

    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void GetUserTests(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user tests:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
        jsonRes.push_back(to_string(i));

    res.set_content(jsonRes.dump(), "application/picojson");


    cout << "   End." << endl;
}
void GetUserRoles(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user roles:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;

    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 5; i++)
        jsonRes.push_back("Role" + to_string(i));

    res.set_content(jsonRes.dump(), "application/picojson");


    cout << "   End." << endl;
}

void SetUserRoles(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set user roles:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    cout << "   End." << endl;
}
void GetUserBlock(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user block:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;

    picojson::picojson jsonRes;
    jsonRes["banned"] = false;
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}

void SetUserBlock(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set user block:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int id = stoi(req.matches[1]);
    cout << "   User " << to_string(id) << " banned." << endl; 
    cout << "   End." << endl;
}
void SetUserUnblock(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set user unblock:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int id = stoi(req.matches[1]);
    cout << "   User " << to_string(id) << " unbanned." << endl; 
    cout << "   End." << endl;
}
void GetDisceplines(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get disceplines:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
    {
        picojson::picojson discepline;
        discepline["name"] = "DiscName" + to_string(i);
        discepline["description"] = "Description" + to_string(i);
        discepline["id"] = i;
        jsonRes.push_back(discepline);
    }
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void GetDisceplineInfo(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get discepline info:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;

    string id = req.matches[1];
    cout << "   id: " << id << endl;

     

     
    picojson::picojson jsonRes;
    jsonRes["name"] = "Disc" + id;
    jsonRes["description"] = "description....";
    jsonRes["prepod"] = "Prepod" + id;

    res.set_content(jsonRes.dump(), "application/picojson");


    cout << "   End." << endl;
}

void SetDisceplineInfo(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set discepline info:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;    
    string id = req.matches[1];
    picojson::picojson body = picojson::picojson::parse(req.body);
    string name = body["name"];
    string description = body["description"];
    cout << "   New name and description: " << name << ' ' << description << endl;
    cout << "   End." << endl;
}
void GetDisceplineTestList(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get discepline tests:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;

    string id = req.matches[1];
    cout << "   id: " << id << endl;
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
        jsonRes.push_back("Test" + to_string(i));
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}

void GetDisceplineTestActive(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get discepline test active:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    string idDisc = req.matches[1];
    string idTest = req.matches[2];
    cout << "   Disc: " << idDisc << " Test: " << idTest << endl;
    picojson::picojson jsonRes;
    jsonRes["active"] = false;
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void SetDisceplineTestActivate(const httplib::Request& req, httplib::Response& res)
{
    cout << "Activate discepline test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    string idDisc = req.matches[1];
    string idTest = req.matches[2];
    cout << "   Active Disc: " << idDisc << " Test: " << idTest << endl;
    cout << "   End." << endl;
}
void SetDisceplineTestDeactivate(const httplib::Request& req, httplib::Response& res)
{
    cout << "Deactivate discepline test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
 string idDisc = req.matches[1];
    string idTest = req.matches[2];
    cout << "   Deactive Disc: " << idDisc << " Test: " << idTest << endl;
    cout << "   End." << endl;
}
void AddDisceplineTest(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add discepline test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    string id = req.matches[1];
    cout << "   id: " << id << endl;
    picojson::picojson body = picojson::picojson::parse(req.body);
    string name = body["name"];
    picojson::picojson jsonRes;
    jsonRes["id"] = 1234;
    cout << "   Add new test \"" << name << "\" id: " << 1234 << endl;
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void DelDisceplineTest(const httplib::Request& req, httplib::Response& res)
{
    cout << "Del discepline test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    string idDisc = req.matches[1];
    string idTest = req.matches[2];
    cout << "   Disc: " << idDisc << " Test: " << idTest << endl;
    cout << "   End." << endl;
}
void GetDisceplineUserList(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get discepline user-list:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    string id = req.matches[1];
    cout << "id: " << id << endl;     
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
        jsonRes.push_back("User" + to_string(i));
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}  
void AddDisceplineUser(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add discepline test user:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    string idDisc = req.matches[1];
    string idUser = req.matches[2];
    cout << "   Disc: " << idDisc << " User: " << idUser << endl;
    cout << "   End." << endl;
}
void DelDisceplineUser(const httplib::Request& req, httplib::Response& res)
{
    cout << "Del discepline test user:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    string idDisc = req.matches[1];
    string idUser = req.matches[2];
    cout << "   Disc: " << idDisc << " User: " << idUser << endl;
    cout << "   End." << endl;
}
void AddDiscepline(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add discepline:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson body = picojson::picojson::parse(req.body);
    string name = body["name"];
    string description = body["description"];
    int idPrepod = stoi(string(body["id"]));
    cout << "   New discepline: " << name << ' ' << description << ' ' << idPrepod << endl;
    cout << "   End." << endl;
}
void DelDiscepline(const httplib::Request& req, httplib::Response& res)
{
    cout << "Del discepline:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    string id = req.matches[1];
    cout << "   id: " << id << endl;
    cout << "   End." << endl;
}
void GetQuestList(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get quest list:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson jsonRes = picojson::picojson::array();
    for (int i = 0; i < 10; i++)
    {
        picojson::picojson discepline;
        discepline["name"] = "quest name" + to_string(i);
        discepline["version"] = to_string(i);
        discepline["idAuthor"] = i;
        jsonRes.push_back(discepline);
    }
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void GetQuestInfo(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get quest info by id:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;   
    string idQuest = req.matches[1];
    string version = req.matches[2];
    cout << "   idQuest: " << idQuest << " version: " << version << endl;
    picojson::picojson jsonRes;
    jsonRes["name"] = "name_quest";
    jsonRes["description"] = "description";
    jsonRes["options"] = picojson::picojson::array();
    for (int i = 0; i < 5; i++)
        jsonRes["options"].push_back("opt" + to_string(i));
    jsonRes["correct_answer"] = 1;
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void SetQuestInfo(const httplib::Request& req, httplib::Response& res)
{
cout << "Set quest info:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;  
    int id = stoi(req.matches[1]);
    picojson::picojson body = picojson::picojson::parse(req.body);
    string name = body["name"];
    string description = body["description"];
    vector<string> options;
    for (const auto i : body["options"])
        options.push_back(i);
    int correct_answer = body["correct_answer"];
    cout << "   End." << endl;
}
void AddQuest(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add quest:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    picojson::picojson body = picojson::picojson::parse(req.body);
    string name = body["name"];
    string description = body["description"];
    vector<string> options;
    for (const auto i : body["options"])
        options.push_back(i);
    int correct_answer = body["correct_answer"];
    cout << "   End." << endl;
}
void DelQuest(const httplib::Request& req, httplib::Response& res)
{
    cout << "Del quest:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    cout << "   End." << endl;
}
void DelTestQuest(const httplib::Request& req, httplib::Response& res)
{
    cout << "Del quest from test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;     
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    int idQuest = stoi(req.matches[3]);

    cout << "   Del test " << idTest << " quest " << idQuest << endl;


    cout << "   End." << endl;
}
void AddTestQuest(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add quest to test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    int idQuest = stoi(req.matches[3]);
    cout << "   End." << endl;
}
void SetTestQuestSequence(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set quest sequence in test:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    cout << "   End." << endl;
}
void GetQuestUsers(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get quest users:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    picojson::picojson jsonRes;
    jsonRes["users"] = picojson::picojson::array();
    for (int i = 0; i < 5; i++)
        jsonRes["users"].push_back("user" + to_string(i));
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void GetTestGreads(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get user gread:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    picojson::picojson jsonRes;
    jsonRes["users"] = picojson::picojson::array();
    for (int i = 0; i < 5; i++)
    {
        picojson::picojson user;
        user["grade"] = 100;
        user["id"] = i;
        jsonRes["users"].push_back(user);
    }
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void GetTestAnswers(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get test ansvers:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    picojson::picojson jsonRes;
    jsonRes["users"] = picojson::picojson::array();
    for (int i = 0; i < 5; i++)
    {
        picojson::picojson user;
        user["answer"] = picojson::picojson::array();
        for (int j = 5; j < 5; j++)
            user["answer"].push_back(i);
        user["id"] = i;
        jsonRes["users"].push_back(user);
    }
    res.set_content(jsonRes.dump(), "application/picojson");
    cout << "   End." << endl;
}
void AddAttempt(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add attempt:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    cout << "   End." << endl;
}
void SetAttempt(const httplib::Request& req, httplib::Response& res)
{
    cout << "Set attempt:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    cout << "   End." << endl;
}
void FinAttempt(const httplib::Request& req, httplib::Response& res)
{
    cout << "Finish attempt:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    cout << "   End." << endl;
}
void GetAttempt(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get attempt:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;  
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    cout << "   End." << endl;
}
void AddAnswer(const httplib::Request& req, httplib::Response& res)
{
    cout << "Add answer:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    int idQest = stoi(req.matches[3]);
    cout << "   End." << endl;
}
void GetAnswer(const httplib::Request& req, httplib::Response& res)
{
    cout << "Get answer:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    int idQest = stoi(req.matches[3]);
    picojson::picojson jsonRes;
    jsonRes["id"] = 0;
    jsonRes["answer"] = -1;
    cout << "   End." << endl;
}
void ChangeAnswer(const httplib::Request& req, httplib::Response& res)
{
    cout << "Change answer:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return;   
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    int idQest = stoi(req.matches[3]);
    cout << "   End." << endl;
}
void DelAnswer(const httplib::Request& req, httplib::Response& res)
{
    cout << "Del answer:" << endl;
    auto permission = CheckToken(req);
    if (Unauthorized(res, permission)) return; 
    int idDisc = stoi(req.matches[1]);
    int idTest = stoi(req.matches[2]);
    int idQest = stoi(req.matches[3]);
    cout << "   End." << endl;
}

