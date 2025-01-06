
_all_ = ['register_user_commands','bot_commands']
from aiogram.filters import Command
from bot.commands.start import start
from aiogram import Router
from aiogram.filters.command import CommandStart
from bot.commands.help import help_command
from bot.commands.help import help_func
from aiogram import F
from bot.commands.unknown import unknown

def register_user_commands(router: Router) -> None:
    router.message.register(start, CommandStart())
    router.message.register(help_command, Command(commands=['help']))
    router.message.register(start, F.text == 'Старт')
    router.message.register(help_func, F.text == 'Помощь')
    router.message.register(help_func, F.text == 'help')
    router.message.register(unknown)