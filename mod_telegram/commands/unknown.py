from aiogram.filters import CommandObject
from aiogram import types
from bot.commands.bot_commands import bot_commands

async def unknown(message: types.Message):
    await message.answer('нет такой команды')