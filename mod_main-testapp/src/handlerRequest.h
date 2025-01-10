#pragma once
#include "checkToken.h"
#include <httplib.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <vector>
void AddUser(const httplib::Request& req, httplib::Response& res);
//      пользователи    
void GetUserList(const httplib::Request& req, httplib::Response& res);                  //  Посмотреть список пользователей
void GetUserNamea(const httplib::Request& req, httplib::Response& res);                 //  Посмотреть информацию о пользователе (ФИО)
void SetUserName(const httplib::Request& req, httplib::Response& res);                  //  Изменить ФИО пользователя
void GetUserCourses(const httplib::Request& req, httplib::Response& res);               //  Посмотреть информацию о пользователе (курсы)              
void GetUserGrades(const httplib::Request& req, httplib::Response& res);                //  Посмотреть информацию о пользователе (оценки)           
void GetUserTests(const httplib::Request& req, httplib::Response& res);                 //  Посмотреть информацию о пользователе (тесты)           
void GetUserRoles(const httplib::Request& req, httplib::Response& res);                 //  Посмотреть информацию о пользователе (роли)                  
void SetUserRoles(const httplib::Request& req, httplib::Response& res);                 //  Изменить роли пользователя              
void GetUserBlock(const httplib::Request& req, httplib::Response& res);                 //  Посмотреть заблокирован ли пользователь               
void SetUserBlock(const httplib::Request& req, httplib::Response& res);                 //  заблокировать пользователя
void SetUserUnblock(const httplib::Request& req, httplib::Response& res);               //  разблокировать пользователя
//      дисциплины         
void GetDisceplines(const httplib::Request& req, httplib::Response& res);               //  Посмотреть список дисциплин
void GetDisceplineInfo(const httplib::Request& req, httplib::Response& res);            //  Посмотреть информацию о дисциплине (Название, Описание, ID преподавателя)
void SetDisceplineInfo(const httplib::Request& req, httplib::Response& res);            //  Изменить информацию о дисциплине (Название, Описание)
void GetDisceplineTestList(const httplib::Request& req, httplib::Response& res);        //  Посмотреть информацию о дисциплине (Список тестов)
void GetDisceplineTestActive(const httplib::Request& req, httplib::Response& res);      //  Посмотреть информацию о тесте (Активный тест или нет)
void SetDisceplineTestActivate(const httplib::Request& req, httplib::Response& res);    //  Активировать/Деактивировать тест
void SetDisceplineTestDeactivate(const httplib::Request& req, httplib::Response& res);  //  Активировать/Деактивировать тест
void AddDisceplineTest(const httplib::Request& req, httplib::Response& res);            //  Добавить тест в дисциплину по её id
void DelDisceplineTest(const httplib::Request& req, httplib::Response& res);            //  Удалить тест из дисциплины (id дисциплины и теста)
void GetDisceplineUserList(const httplib::Request& req, httplib::Response& res);        //  Посмотреть информацию о дисциплине (Список студентов)
void AddDisceplineUser(const httplib::Request& req, httplib::Response& res);            //  Записать пользователя на дисциплину
void DelDisceplineUser(const httplib::Request& req, httplib::Response& res);            //  Отчислить пользователя с дисциплины
void AddDiscepline(const httplib::Request& req, httplib::Response& res);                //  Создать дисциплину
void DelDiscepline(const httplib::Request& req, httplib::Response& res);                //  Удалить дисциплину
//      вопросы             
void GetQuestList(const httplib::Request& req, httplib::Response& res);                 //  Посмотреть список вопросов
void GetQuestInfo(const httplib::Request& req, httplib::Response& res);                 //  Посмотреть информацию о вопросе id вопроса и id версии вопроса
void SetQuestInfo(const httplib::Request& req, httplib::Response& res);                 //  Изменить текст вопроса/ответов (создаётся новая версия)
void AddQuest(const httplib::Request& req, httplib::Response& res);                     //  Создать вопрос
void DelQuest(const httplib::Request& req, httplib::Response& res);                     //  Удалить вопрос
//      Тесты          
void DelTestQuest(const httplib::Request& req, httplib::Response& res);                 //  Удалить вопрос из теста
void AddTestQuest(const httplib::Request& req, httplib::Response& res);                 //  Добавить вопрос в тест
void SetTestQuestSequence(const httplib::Request& req, httplib::Response& res);         //  Изменить порядок следования вопросов в тесте
void GetQuestUsers(const httplib::Request& req, httplib::Response& res);                //  Посмотреть список пользователей прошедших тест
void GetTestGreads(const httplib::Request& req, httplib::Response& res);                //  Посмотреть оценку пользователя
void GetTestAnswers(const httplib::Request& req, httplib::Response& res);               //  Посмотреть ответы пользователя
//      Попытки       
void AddAttempt(const httplib::Request& req, httplib::Response& res);                   //  Создать
void SetAttempt(const httplib::Request& req, httplib::Response& res);                   //  Изменить
void FinAttempt(const httplib::Request& req, httplib::Response& res);                   //  Завершить
void GetAttempt(const httplib::Request& req, httplib::Response& res);                   //  Посмотреть
//      Ответы            
void AddAnswer(const httplib::Request& req, httplib::Response& res);                    //  Создать
void GetAnswer(const httplib::Request& req, httplib::Response& res);                    //  Посмотреть
void ChangeAnswer(const httplib::Request& req, httplib::Response& res);                 //  Изменить
void DelAnswer(const httplib::Request& req, httplib::Response& res);                    //  Удалить
