from aiogram.filters import CommandObject
from aiogram import types
from bot.commands.bot_commands import bot_commands

async def help_command(message: types.Message, command: CommandObject):
    if command.args:
        for cmd in bot_commands:
            if cmd[0] == command.args:
                return await message.answer(
                    f' {cmd[0]} -{cmd[1]}\n\n{cmd[3]}'
                )
            else:
                return await message.answer('команда не найдена')
        return help_func(message)
    await message.answer('ПАМАГИТЕ')
async def help_func(message: types.Message):
    return await message.answer(
        'помощь и справка о боте\n'
        'для того, чтобы получить информацию о команде используйте /help <команда>\n'
    )

















