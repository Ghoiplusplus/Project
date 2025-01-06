import os
import asyncio
import logging
from aiogram import Dispatcher, Bot
import redis
from aiogram.types import BotCommand


from bot.commands.bot_commands import bot_commands
from commands import *


r = redis.Redis(host='127.0.0.1', port=6379, db=0)
try:
    info = r.info()
    print(info['redis_version'])
    response = r.ping()
    if response:
        print("Подключение успешно!")
    else:
        print("Не удалось подключиться к Redis.")
except redis.exceptions.RedisError as e:
    print(f"Ошибка: {e}")

async def main() -> None:
    logging.basicConfig(level=logging.DEBUG)


    commands_for_bot = []
    for cmd in bot_commands:
        commands_for_bot.append(BotCommand(command=cmd[0], description=cmd[1]))

    dp = Dispatcher()
    bot = Bot(token=os.getenv('token'))
    await bot.set_my_commands(commands_for_bot)
    register_user_commands(dp)

    await dp.start_polling(bot)


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except (KeyboardInterrupt, SystemExit):
        print('Bot stopped by user')
























