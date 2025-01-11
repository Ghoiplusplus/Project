import asyncio
from aiogram import Bot, Dispatcher, types, F
from aiogram.filters import Command
from handlers.commands import (
    start_command, login_command, logout_command, list_tests,
    test_details_command, create_attempt_command, submit_answer_command,
    get_results_command, help_command, auth_button, tests_button, help_button_command, git_button, code_button,
    yandex_button, get_test_questions, get_test_questions_math, get_test_questions_history
)
from handlers.keyboards import get_main_keyboard

bot = Bot(token="7905749771:AAELu7FPG_mZgIrOlcgBUHlGtYXhW-PDRgM")
dp = Dispatcher()

dp.message.register(start_command, Command(commands=['start']))
dp.message.register(login_command, Command(commands=['login']))
dp.message.register(logout_command, Command(commands=['logout']))
dp.message.register(help_command, Command(commands=['help']))
dp.message.register(list_tests, Command(commands=['show_tests']))
dp.message.register(test_details_command, Command(commands=['test_details']))
dp.message.register(create_attempt_command, Command(commands=['create_attempt']))
dp.message.register(submit_answer_command, Command(commands=['submit_answer']))
dp.message.register(get_results_command, Command(commands=['get_results']))
dp.message.register(get_test_questions_math, Command(commands=['get_test_questions_math']))
dp.message.register(get_test_questions_history, Command(commands=['get_test_questions_history']))

dp.message.register(auth_button, lambda message: message.text == 'Авторизация')
dp.message.register(tests_button, lambda message: message.text == 'Тесты')
dp.message.register(help_button_command, lambda message: message.text == 'Помощь')
dp.message.register(git_button, lambda message: message.text == 'Github')
dp.message.register( yandex_button, lambda message: message.text == 'yandex')
dp.message.register(code_button, lambda message: message.text == 'code')

async def unknown_command(message: types.Message):
    if message.text.startswith('/'):
        await message.answer("Такая команда недоступна.")

dp.message.register(unknown_command, F.text.startswith('/'))

async def unknown_message(message: types.Message):
    await message.answer("Сообщение не распознано. Используйте команду из списка /help или же воспользуйтесь клавиатурой.")

dp.message.register(unknown_message, ~F.text.in_({'Авторизация', 'Тесты', 'Помощь'}))

async def main():
    try:
        await dp.start_polling(bot)
    except Exception as e:
        print(f"Ошибка: {e}")

if __name__ == '__main__':
    asyncio.run(main())