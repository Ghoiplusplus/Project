package project.webclient.service;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.WebClient;

@Service
public class AuthModuleClient {
    private final WebClient webClient;

    public AuthModuleClient(
            @Value("${services.auth-module}") String authServerUrl
    ) {
        this.webClient = WebClient.builder()
                .baseUrl(authServerUrl)
                .build();
    }

    /**
     * Отправить запрос на модуль Авторизации для начала входа.
     * Возвращаем URL/redirect, который надо будет показать пользователю
     */
    public String startLogin(String loginToken, String type) {
        // Пример запроса GET: /authorize?type=github&token=loginToken
        // В реальном мире: ждем JSON с полями { redirectUrl: "https://..." } и т.п.
        return this.webClient.get()
                .uri(uriBuilder -> uriBuilder
                        .path("/authorize")
                        .queryParam("type", type)
                        .queryParam("token", loginToken)
                        .build())
                .retrieve()
                .bodyToMono(String.class)
                .block();
    }

    /**
     * Проверить текущее состояние loginToken в модуле Авторизации
     * (на случай, если пользователь уже подтвердил/отказал).
     * Возвращает одно из:
     * - "NOT_RECOGNIZED"
     * - "DENIED"
     * - "GRANTED"
     * ... и, возможно, сами токены, если GRANTED.
     */
    public AuthCheckResult checkLoginToken(String loginToken) {
        // Допустим, есть эндпоинт /checkLoginToken?token=...
        return this.webClient.get()
                .uri(uriBuilder -> uriBuilder
                        .path("/checkLoginToken")
                        .queryParam("token", loginToken)
                        .build())
                .retrieve()
                .bodyToMono(AuthCheckResult.class)
                .block();
    }

    /**
     * Обновить accessToken при помощи refreshToken
     * Возвращает новые токены или 401, если refreshToken устарел.
     */
    public TokenRefreshResult refreshAccessToken(String refreshToken) {
        return this.webClient.post()
                .uri("/refresh")
                .bodyValue(new RefreshRequest(refreshToken))
                .retrieve()
                .bodyToMono(TokenRefreshResult.class)
                .block();
    }

    /**
     * Вызов /logout на модуль Авторизации, чтобы аннулировать refreshToken
     */
    public void logoutEverywhere(String refreshToken) {
        this.webClient.post()
                .uri("/logout")
                .bodyValue(new RefreshRequest(refreshToken))
                .retrieve()
                .bodyToMono(Void.class)
                .block();
    }

    // ===================== Вспомогательные DTO ====================

    public static class RefreshRequest {
        public String refreshToken;
        public RefreshRequest(String refreshToken) {
            this.refreshToken = refreshToken;
        }
    }

    public static class TokenRefreshResult {
        public String accessToken;
        public String refreshToken;
        public boolean success;
        // геттеры/сеттеры
    }

    public static class AuthCheckResult {
        public String status; // NOT_RECOGNIZED / DENIED / GRANTED
        public String accessToken;
        public String refreshToken;
        // геттеры/сеттеры
    }
}
