
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <string>
#include <iostream>

#include "handlerRequest.h"
#include "PostgreSQL.h"


int main ()
{

//      ПОДКЛЮЧЕНИЕ К PostgreSQL
    PostgresInit();


//      ДОБАВЛЕНИЕ ЭНДПОИНТОВ И НАЧАЛО ПРОСЛУШКИ ПОРТА


    httplib::Server server;

    server.Post("api/db/addUser", AddUser);         //      добавляет нового пользователя в бд после регистрации

//      пользователи
    server.Get("/api/db/users", GetUserList);                                   //  Посмотреть список пользователей
    server.Get("/api/db/users/(\\d+)/name", GetUserNamea);                      //  Посмотреть информацию о пользователе (ФИО)
    server.Put("/api/db/users/(\\d+)/name", SetUserName);                       //  Изменить ФИО пользователя
    server.Get("/api/db/users/(\\d+)/courses", GetUserCourses);                 //  Посмотреть информацию о пользователе (курсы)
    server.Get("/api/db/users/(\\d+)/grades", GetUserGrades);                   //  Посмотреть информацию о пользователе (оценки)
    server.Get("/api/db/users/(\\d+)/tests", GetUserTests);                     //  Посмотреть информацию о пользователе (тесты)
    server.Get("/api/db/users/(\\d+)/roles", GetUserRoles);                     //  Посмотреть информацию о пользователе (роли)
    server.Put("/api/db/users/(\\d+)/roles", SetUserRoles);                     //  Изменить роли пользователя    
    server.Get("/api/db/users/(\\d+)/block", GetUserBlock);                     //  Посмотреть заблокирован ли пользователь
    server.Put("/api/db/users/(\\d+)/block", SetUserBlock);                     //  заблокировать пользователя
    server.Put("/api/db/users/(\\d+)/unblock", SetUserUnblock);                 //  разблокировать пользователя
// ..............................

//      дисциплины
    server.Get("/api/db/disciplines", GetDisceplines);                                                  //  Посмотреть список дисциплин
    server.Get("/api/db/disciplines/(\\d+)", GetDisceplineInfo);                                        //  Посмотреть информацию о дисциплине (Название, Описание, ID преподавателя)
    server.Put("/api/db/disciplines/(\\d+)", SetDisceplineInfo);                                        //  Изменить информацию о дисциплине (Название, Описание)
    server.Get("/api/db/disciplines/(\\d+)/tests", GetDisceplineTestList);                              //  Посмотреть информацию о дисциплине (Список тестов) по её id
    server.Get("/api/db/disciplines/(\\d+)/tests/(\\d+)/active", GetDisceplineTestActive);              //  Посмотреть информацию о тесте (Активный тест или нет) (id теста и дисциплины)
    server.Put("/api/db/disciplines/(\\d+)/tests/(\\d+)/activate", SetDisceplineTestActivate);          //  Активировать тест
    server.Put("/api/db/disciplines/(\\d+)/tests/(\\d+)/deactivate", SetDisceplineTestDeactivate);      //  Деактивировать тест
    server.Post("/api/db/disciplines/(\\d+)/tests", AddDisceplineTest);                                 //  Добавить тест в дисциплину по её id
    server.Delete("/api/db/disciplines/(\\d+)/tests/(\\d+)", DelDisceplineTest);                        //  Удалить тест из дисциплины (id дисциплины и теста)
    server.Get("/api/db/disciplines/(\\d+)/users", GetDisceplineUserList);                              //  Посмотреть информацию о дисциплине (Список студентов)    
    server.Put("/api/db/disciplines/(\\d+)/users(\\d+)", AddDisceplineUser);                            //  Записать пользователя на дисциплину
    server.Delete("/api/db/disciplines/(\\d+)/users(\\d+)", DelDisceplineUser);                         //  Отчислить пользователя с дисциплины
    server.Post("/api/db/disciplines", AddDiscepline);                                                  //  Создать дисциплину
    server.Delete("/api/db/disciplines/(\\d+)", DelDiscepline);                                         //  Удалить дисциплину
// ..............................

//      вопросы                                                      
    server.Get("/api/db/quest", GetQuestList);                                                  //  Посмотреть список вопросов
    server.Get("/api/db/quest/(\\d+)/(\\d+)", GetQuestInfo);                                    //  Посмотреть информацию о вопросе id вопроса и id версии вопроса
    server.Put("/api/db/quest(\\d+)", SetQuestInfo);                                            //  Изменить текст вопроса/ответов (создаётся новая версия)
    server.Post("/api/db/quest", AddQuest);                                                     //  Создать вопрос
    server.Delete("/api/db/quest(\\d+)", DelQuest);                                             //  Удалить вопрос
//  ..............................

//      тесты 
    server.Delete("/api/db/disciplines/(\\d+)/tests/(\\d+)/quest(\\d+)", DelTestQuest);         //  Удалить вопрос из теста
    server.Post("/api/db/disciplines/(\\d+)/tests/(\\d+)/quest(\\d+)", AddTestQuest);           //  Добавить вопрос в тест
    server.Put("/api/db/disciplines/(\\d+)/tests/(\\d+)/questSequence", SetTestQuestSequence);  //  Изменить порядок следования вопросов в тесте
    server.Get("/api/db/disciplines/(\\d+)/tests/(\\d+)/users", GetQuestUsers);                 //  Посмотреть список пользователей прошедших тест
    server.Get("/api/db/disciplines/(\\d+)/tests/(\\d+)/gread", GetQuestUsers);                 //  Посмотреть оценку пользователя
    server.Get("/api/db/disciplines/(\\d+)/tests/(\\d+)/answer", GetTestAnswers);               //  Посмотреть ответы пользователя
//  ..............................

//      попытки
    server.Post("/api/db/disciplines/(\\d+)/test/(\\d+)/attempt", AddAttempt);                  //  Создать
    server.Put("/api/db/disciplines/(\\d+)/test/(\\d+)/attempt", SetAttempt);                   //  Изменить
    server.Post("/api/db/disciplines/(\\d+)/test/(\\d+)/attempt/finish", FinAttempt);           //  Завершить
    server.Get("/api/db/disciplines/(\\d+)/test/(\\d+)/attempt", GetAttempt);                   //  Посмотреть
//  ..............................

//      ответы
    server.Post("/api/db/disciplines/(\\d+)/test/(\\d+)/quest/(\\d+)", AddAnswer);              //  Создать
    server.Get("/api/db/disciplines/(\\d+)/test/(\\d+)/quest/(\\d+)", GetAnswer);               //  Посмотреть
    server.Put("/api/db/disciplines/(\\d+)/test/(\\d+)/quest/(\\d+)", ChangeAnswer);            //  Изменить
    server.Delete("/api/db/disciplines/(\\d+)/test/(\\d+)/quest/(\\d+)", DelAnswer);            //  Удалить
//  ..............................


    std::cout << "Server started at http://127.0.0.1:1111/" << std::endl << std::endl;
    server.listen("127.0.0.1", 1111);
    std::cout << "Server was stoped." << std::endl;

    return 0;
}
