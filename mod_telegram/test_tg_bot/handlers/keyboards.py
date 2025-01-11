from aiogram.types import ReplyKeyboardMarkup, KeyboardButton

def get_main_keyboard(is_authorized: bool = False):
    if is_authorized:
        keyboard = [
            [KeyboardButton(text="Помощь")]
        ]
    else:
        keyboard = [
            [KeyboardButton(text="Помощь")],
            [KeyboardButton(text="Github")],
            [KeyboardButton(text="yandex")],
            [KeyboardButton(text="code")]
        ]
    print(f"Keyboard generated (is_authorized={is_authorized}): {keyboard}")
    return ReplyKeyboardMarkup(keyboard=keyboard, resize_keyboard=True, one_time_keyboard=False)