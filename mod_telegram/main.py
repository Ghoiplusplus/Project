import os
import asyncio
import logging
from aiogram import Dispatcher, Bot

from aiogram.types import BotCommand
from sqlalchemy import URL

from bot.commands.bot_commands import bot_commands
from commands import *

from db import BaseModel , create_async_engine, proceed_schemas, get_session_maker



async def main() -> None:
    logging.basicConfig(level=logging.DEBUG)


    commands_for_bot = []
    for cmd in bot_commands:
        commands_for_bot.append(BotCommand(command=cmd[0], description=cmd[1]))

    dp = Dispatcher()
    bot = Bot(token=os.getenv('token'))
    await bot.set_my_commands(commands_for_bot)
    register_user_commands(dp)

    postgres_url = URL.create(
        "postgresql+asyncpg",
        username=os.getenv('db_user'),
        host="localhost",
        database=os.getenv('db_name'),
        port=os.getenv('db_port')
    )

    async_engine = create_async_engine(postgres_url)
    session_maker = get_session_maker(async_engine)
    await proceed_schemas(async_engine, BaseModel.metadata)

    await dp.start_polling(bot)


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except (KeyboardInterrupt, SystemExit):
        print('Bot stopped by user')
























