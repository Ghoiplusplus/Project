# проверка токена
def check_token(token):
    if token == "valid_token":
        return {"status": "success", "access_token": "mock_access_token", "refresh_token": "mock_refresh_token"}
    else:
        return {"status": "error", "message": "Invalid token"}

# выход из системы
def logout(refresh_token):
    return {"status": "success", "message": "Logged out"}

# авторизация
def authorize_user(login_token):
    if login_token:
        return {"status": "success", "access_token": "mock_access_token", "refresh_token": "mock_refresh_token"}
    else:
        return {"status": "error", "message": "Authorization failed"}