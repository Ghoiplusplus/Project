package project.webclient.service;

import org.springframework.stereotype.Service;
import project.webclient.model.SessionData;
import project.webclient.repository.SessionRepository;

@Service
public class SessionService {

    private final SessionRepository sessionRepository;
    private final AuthModuleClient authClient;
    private final MainModuleClient mainClient;

    public SessionService(SessionRepository sessionRepository,
                          AuthModuleClient authClient,
                          MainModuleClient mainClient) {
        this.sessionRepository = sessionRepository;
        this.authClient = authClient;
        this.mainClient = mainClient;
    }

    public SessionData getOrCreateSession(String sessionToken) {
        SessionData data = sessionRepository.getSession(sessionToken);
        if (data == null) {
            // Пользователь неизвестен
            data = new SessionData("UNKNOWN", null, null, null);
            sessionRepository.saveSession(sessionToken, data);
        } else {
            // Обновим TTL
            sessionRepository.updateSessionTTL(sessionToken);
        }
        return data;
    }

    public void removeSession(String sessionToken) {
        sessionRepository.deleteSession(sessionToken);
    }

    /**
     * Начать процедуру логина:
     *  1) генерируем новый loginToken
     *  2) складываем в Redis (статус = ANONYMOUS)
     *  3) идём в AuthModuleClient.startLogin(...)
     *  4) возвращаем redirectURL (или саму ссылку)
     */
    public String startLogin(String sessionToken, String type) {
        SessionData data = new SessionData("ANONYMOUS", generateRandomLoginToken(), null, null);
        sessionRepository.saveSession(sessionToken, data);

        return authClient.startLogin(data.getLoginToken(), type);
    }

    /**
     * Проверяем токен входа: созрел ли он (DENIED/GRANTED),
     * и если GRANTED, то сохраняем токены и статус = AUTHORIZED
     */
    public void checkLoginTokenAndUpdateSession(String sessionToken) {
        SessionData data = sessionRepository.getSession(sessionToken);
        if (data == null || data.getUserState() == null) {
            return; // сессии нет - неизвестный пользователь
        }
        if (!"ANONYMOUS".equalsIgnoreCase(data.getUserState())) {
            return; // уже не в состоянии ожидания
        }
        String loginToken = data.getLoginToken();
        if (loginToken == null) {
            return; // нет loginToken
        }

        AuthModuleClient.AuthCheckResult result = authClient.checkLoginToken(loginToken);
        switch (result.status) {
            case "NOT_RECOGNIZED":
                // Если время действия токена входа вышло
                // Считаем пользователя UNKNOWN
                sessionRepository.deleteSession(sessionToken);
                break;
            case "DENIED":
                // Пользователь нажал "Нет" во время входа
                sessionRepository.deleteSession(sessionToken);
                break;
            case "GRANTED":
                // Доступ предоставлен
                data.setUserState("AUTHORIZED");
                data.setAccessToken(result.accessToken);
                data.setRefreshToken(result.refreshToken);
                // обнулим loginToken
                data.setLoginToken(null);
                sessionRepository.saveSession(sessionToken, data);
                break;
        }
    }

    /**
     * Выход только на данном устройстве
     */
    public void logoutLocal(String sessionToken) {
        sessionRepository.deleteSession(sessionToken);
    }

    /**
     * Выход на всех устройствах
     */
    public void logoutAll(String sessionToken) {
        SessionData data = sessionRepository.getSession(sessionToken);
        if (data != null && data.getRefreshToken() != null) {
            // говорим модулю авторизации аннулировать refreshToken
            authClient.logoutEverywhere(data.getRefreshToken());
        }
        sessionRepository.deleteSession(sessionToken);
    }

    /**
     * Выполнить действие (GET /{actionPath}) через Главный модуль.
     * При 401 пытаемся рефрешить токен.
     */
    public String doAction(String sessionToken, String actionPath) {
        SessionData data = sessionRepository.getSession(sessionToken);
        if (data == null || !"AUTHORIZED".equalsIgnoreCase(data.getUserState())) {
            // редирект на главную
            return "redirect:/";
        }

        try {
            // вызов в Главный модуль
            MainModuleClient.MainModuleResponse resp = mainClient.doAction(data.getAccessToken(), actionPath);
            // вернём данные (на практике – отрендерим HTML-страницу)
            return "Action result: " + resp.data;
        } catch (MainModuleClient.UnauthorizedException e) {
            // 401. Пробуем рефрешить:
            boolean refreshOk = tryRefreshTokens(data);
            if (!refreshOk) {
                // Становимся UNKNOWN
                sessionRepository.deleteSession(sessionToken);
                return "redirect:/";
            }
            // Повторяем действие
            return doAction(sessionToken, actionPath);
        } catch (MainModuleClient.ForbiddenException e) {
            // 403 - нет прав
            return "403 - Доступ запрещён";
        }
    }

    /**
     * Попытка рефреша accessToken
     */
    private boolean tryRefreshTokens(SessionData data) {
        if (data.getRefreshToken() == null) {
            return false;
        }
        AuthModuleClient.TokenRefreshResult result = authClient.refreshAccessToken(data.getRefreshToken());
        if (result == null || !result.success) {
            return false;
        }
        data.setAccessToken(result.accessToken);
        data.setRefreshToken(result.refreshToken);
        sessionRepository.saveSession(data.getAccessToken(), data);
        return true;
    }

    private String generateRandomLoginToken() {
        return java.util.UUID.randomUUID().toString();
    }
}
