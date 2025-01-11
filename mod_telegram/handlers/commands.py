from multiprocessing.util import sub_warning
import requests
import json
import time
from requests.exceptions import RequestException
from aiogram import types
from aiogram.filters import Command
from handlers.keyboards import get_main_keyboard
from mocks.auth_mock import check_token, logout, authorize_user
from mocks.main_module_mock import get_tests, get_test_details, get_test_questions, create_attempt, submit_answer, get_attempt_results
from services.redis_service import set_user_status, get_user_status, set_user_token, delete_user_session, get_user_token
import uuid

#/start
async def start_command(message: types.Message):
    chat_id = message.chat.id
    status = get_user_status(chat_id)
    print(f"/start called with status: {status}")  # Отладочный вывод
    if not status:
        set_user_status(chat_id, 'Неизвестный')
    await message.answer(
        "Привет, я бот для тестов ",
        reply_markup=get_main_keyboard()
    )
async def login_command(message: types.Message):
    chat_id = message.chat.id
    status = get_user_status(chat_id)
    print(f"/login called with status: {status}")  # Отладочный вывод
    if status == 'Неизвестный':
        set_user_status(chat_id, 'Анонимный')
        login_token = str(uuid.uuid4())
        set_user_token(chat_id, login_token)
        response = authorize_user(login_token)
        print(f"Authorization response: {response}")  # Отладочный вывод
        if response["status"] == "success":
            set_user_status(chat_id, "Авторизованный")
            print(f"User {chat_id} status updated to: Авторизованный")  # Отладочный вывод
            keyboard = get_main_keyboard(is_authorized=True)
            await message.answer(
                "Вы успешно авторизовались!",
                reply_markup=keyboard
            )
        else:
            await message.answer(" Ошибка авторизации.")
    elif status == 'Анонимный':
        await message.answer("Вы уже начали процесс авторизации. Пожалуйста, завершите его.")
    elif status == 'Авторизованный':
        await message.answer(" Вы уже авторизованы. ")

#/logout
async def logout_command(message: types.Message):
    chat_id = message.chat.id
    refresh_token = get_user_token(chat_id)
    print(f"/logout called with token: {refresh_token}")  # Отладочный вывод
    if refresh_token:
        response = logout(refresh_token)
        print(f"Logout response: {response}")  # Отладочный вывод
        if response["status"] == "success":
            delete_user_session(chat_id)
            print(f"User {chat_id} session deleted. New status: {get_user_status(chat_id)}")  # Отладочный вывод
            await message.answer(
                "Вы успешно вышли из системы.",
                reply_markup=get_main_keyboard(is_authorized=False)
            )
    else:
        await message.answer(" Вы не авторизованы. Войдите в систему, чтобы продолжить.")

#/show_tests
async def list_tests(message: types.Message):
    tests = get_tests()
    if not tests:
        await message.answer("Тесты не найдены.")
        return

    response = " Доступные тесты:\n"
    for test in tests:
        response += f"{test['id']}. {test['name']} — {test['description']}\n"

    await message.answer(response)

#/test_details
async def test_details_command(message: types.Message):
    text = message.text.split()
    print(f"/test_details called with args: {text}")  # Отладочный вывод
    if len(text) < 2:
        await message.answer("Пожалуйста, укажите ID теста.")
        return
    
    test_id = int(text[1])
    test = get_test_details(test_id)
    print(f"Test details: {test}")  # Отладочный вывод
    
    if test:
        response = f"Название: {test['name']}\nОписание: {test['description']}"
    else:
        response = "Тест не найден."
    
    await message.answer(response)

#/create_attempt
async def create_attempt_command(message: types.Message):
    text = message.text.split()
    print(f"/create_attempt called with args: {text}")  # Отладочный вывод
    if len(text) < 2:
        await message.answer("Пожалуйста, укажите ID теста.")
        return
    
    test_id = int(text[1])
    user_id = message.from_user.id
    attempt = create_attempt(test_id, user_id)
    print(f"Attempt: {attempt}")  # Отладочный вывод
    
    if attempt:
        response = f"Попытка теста '{test_id}' создана. ID попытки: {attempt['id']}."
    else:
        response = "Ошибка создания попытки."
    
    await message.answer(response)
async def get_test_questions_math(message: types.Message):
    text = message.text.split()
    print(f"/get_test_questions_math called with args: {text}")
    await message.answer("Вопрос 1 : Сколько будет 2 + 2? options: [3, 4, 5]              "
                         
                         "            Вопрос 2 : Сколько будет 3 * 3?, options: [6, 9, 12]")


async def get_test_questions_history(message: types.Message):
    text = message.text.split()
    print(f"/get_test_questions_history called with args: {text}")
    await message.answer("Вопрос 1 : В каком году началась Вторая мировая война?, options: [1939, 1941, 1945]                               "

                         "                                Вопрос 2 : Кто был первым президентом США?, options: [Джордж Вашингтон, Авраам Линкольн, Томас Джефферсон]")
async def submit_answer_command(message: types.Message):
    text = message.text.split()
    if len(text) != 4:
        await message.answer("Используйте: /submit_answer <ID попытки> <ID вопроса> <номер ответа>")
        return
    
    attempt_id, question_id, answer = map(int, text[1:])
    result = submit_answer(attempt_id, question_id, answer)
    
    if result["status"] == "success":
        response = "Ответ принят."
    else:
        response = result["message"]
    
    await message.answer(response)

#/get_results
async def get_results_command(message: types.Message):
    text = message.text.split()
    if len(text) < 2:
        await message.answer("Пожалуйста, укажите ID попытки.")
        return
    
    attempt_id = int(text[1])
    results = get_attempt_results(attempt_id)
    
    if results["status"] == "success":
        response = f"Результаты попытки {attempt_id}:\n"
        for result in results["results"]:
            response += (f"Вопрос: {result['question_text']}\n"
                         f"Ваш ответ: {result['user_answer']}\n"
                         f"Правильный ответ: {result['correct_answer']}\n"
                         f"Правильно: {'Да' if result['is_correct'] else 'Нет'}\n\n")
        response += (f"Всего правильных ответов: {results['total_correct']}\n"
                     f"Всего вопросов: {results['total_questions']}\n")
    else:
        response = results["message"]
    
    await message.answer(response)

#/help
async def help_command(message: types.Message):
    help_text = """
<b><i>Доступные команды:</i></b>\n
/start - Начать работу с ботом
/login - Авторизоваться в боте
/logout - Выйти из системы
/show_tests - Показать список тестов
/test_details id теста - Показать детали теста
/get_test_questions_math - Показать вопросы по математике
/get_test_questions_history - Показать вопросы по истории
/create_attempt *id теста* - Начать попытку теста
/submit_answer *id попытки* *№ вопроса* *№ ответа* - Ответить на вопрос
/get_results *id попытки* - Показать результаты теста
<i>/help</i> - Получить список команд
"""
    await message.answer(help_text, parse_mode="HTML")

#button "Авторизация"
async def auth_button(message: types.Message):
    chat_id = message.chat.id
    status = get_user_status(chat_id)
    print(f"auth_button called with status: {status}")  # Отладочный вывод
    if status == 'Неизвестный':
        await login_command(message)
    elif status == 'Анонимный':
        await message.answer("Вы уже начали процесс авторизации. Пожалуйста, завершите его.")
    elif status == 'Авторизованный':
        await message.answer(" Вы уже авторизованы.")

#button "Тесты"
async def tests_button(message: types.Message):
    chat_id = message.chat.id
    status = get_user_status(chat_id)
    print(f"tests_button called with status: {status}")  # Отладочный вывод
    if status == 'Авторизованный':
        await message.answer("Пусто")
    else:
        await message.answer("Для доступа к тестам необходимо авторизоваться. Используйте команду /login.")

#button "Помощь"
async def help_button_command(message: types.Message):
    await help_command(message)

async def git_button(message: types.Message):
    login_token = str(uuid.uuid4())
    await message.answer("http://127.0.0.1:2228/login?type=github&token=" + login_token)

    try:
        time.sleep(20)
        response = requests.get('http://127.0.0.1:2228/response?state=' + login_token)
        data = response.content.decode('utf-8') # Декодируем данные
        json_data = json.loads(data)
        for x in json_data:
            if "Access Token" in x:
                chat_id = message.chat.id
                status = get_user_status(chat_id)
                set_user_token(chat_id, "Access Token: ")
                if status == 'Неизвестный' or status == 'Анонимный':
                    set_user_status(chat_id, "Авторизованный")
                    print(f"User {chat_id} status updated to: Авторизованный")  # Отладочный вывод
                    keyboard = get_main_keyboard(is_authorized=True)
                    await message.answer(
                        "Вы успешно авторизовались!",
                        reply_markup=keyboard
                    )
                else:
                    await message.answer(" Ошибка авторизации.")
            # else:
                # await message.answer("Ошибка авторизации")
    except RequestException as e:
        print(f"Проблема с выполнением HTTP-запроса: {e}")
    except json.JSONDecodeError:
        print("Не удалось декодировать JSON. Пожалуйста, попробуйте еще раз.")

async def yandex_button(message: types.Message):
    login_token = str(uuid.uuid4())
    await message.answer("http://127.0.0.1:2228/login?type=yandex&token=" + login_token)

    try:
        time.sleep(20)
        response = requests.get('http://127.0.0.1:2228/response?state=' + login_token)
        data = response.content.decode('utf-8') # Декодируем данные
        json_data = json.loads(data)
        for x in json_data:
            if "Access Token" in x:
                chat_id = message.chat.id
                status = get_user_status(chat_id)
                if status == 'Неизвестный' or status == 'Анонимный':
                    set_user_token(chat_id, "Access Token: ")
                    set_user_status(chat_id, "Авторизованный")
                    print(f"User {chat_id} status updated to: Авторизованный")  # Отладочный вывод
                    keyboard = get_main_keyboard(is_authorized=True)
                    await message.answer(
                        "Вы успешно авторизовались!",
                        reply_markup=keyboard
                    )
                else:
                    await message.answer(" Ошибка авторизации.")
    except RequestException as e:
        print(f"Проблема с выполнением HTTP-запроса: {e}")
    except json.JSONDecodeError:
        print("Не удалось декодировать JSON. Пожалуйста, попробуйте еще раз.")



async def code_button(message: types.Message):
    await login_command(message)