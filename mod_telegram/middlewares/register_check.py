# from typing import Callable, Dict, Any, Awaitable
#
# from aiogram import BaseMiddleware
# from aiogram.types import Message
#
#
# class register_check(BaseMiddleware):
#     async def __call__(
#         self,
#         handler: Callable[[Message, Dict[str, Any]], Awaitable[Any]],
#         event: Message,
#         data: Dict[str, Any]
#     ) -> Any:
#
#
#
#         return await handler(event, data)