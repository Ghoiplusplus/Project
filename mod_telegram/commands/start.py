from aiogram import types
from aiogram.types import KeyboardButton, KeyboardButtonPollType, Message
from aiogram.utils.keyboard import (ReplyKeyboardMarkup, InlineKeyboardMarkup, InlineKeyboardBuilder,
                                    KeyboardButton, InlineKeyboardButton, ReplyKeyboardBuilder
                                    )
async def start(message: types.Message) -> None:
    menu_builder = ReplyKeyboardBuilder()
    menu_builder.button(
        text='Помощь'
    )
    menu_builder.add(
        KeyboardButton(text='Отправить контакт',request_contact=True)
    )
    menu_builder.row(
        KeyboardButton(text='Отправить голосование', request_poll=KeyboardButtonPollType())
    )
    await message.answer(
        'Меню',
        reply_markup=menu_builder.as_markup()
    )




































