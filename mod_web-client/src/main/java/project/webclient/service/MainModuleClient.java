package project.webclient.service;

import org.springframework.http.HttpHeaders;
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Mono;

@Service
public class MainModuleClient {

    private final WebClient webClient;

    public MainModuleClient() {
        // адрес Главного модуля
        this.webClient = WebClient.builder()
                .baseUrl("http://main-module:8081") // или другой порт
                .build();
    }

    /**
     * Пример запроса на некий эндпоинт Главного модуля
     */
    public MainModuleResponse doAction(String accessToken, String path) {
        // GET /{path}
        return webClient.get()
                .uri("/" + path)
                .header(HttpHeaders.AUTHORIZATION, "Bearer " + accessToken)
                .retrieve()
                .onStatus(status -> status.value() == 401,
                        response -> Mono.error(new UnauthorizedException("401 from Main Module"))
                )
                .onStatus(status -> status.value() == 403,
                        response -> Mono.error(new ForbiddenException("403 from Main Module"))
                )
                .bodyToMono(MainModuleResponse.class)
                .block();
    }

    // Запрос уведомлений, как пример
    public NotificationsResponse getNotifications(String accessToken) {
        return webClient.get()
                .uri("/notification")
                .header(HttpHeaders.AUTHORIZATION, "Bearer " + accessToken)
                .retrieve()
                .onStatus(status -> status.value() == 401,
                        response -> Mono.error(new UnauthorizedException("401 from Main Module"))
                )
                .bodyToMono(NotificationsResponse.class)
                .block();
    }

    // ... тут другие методы для всех нужных CRUD-действий ...

    // Вспомогательные классы
    public static class MainModuleResponse {
        public String data;
        // ...
    }

    public static class NotificationsResponse {
        public String[] notifications;
        // ...
    }

    public static class UnauthorizedException extends RuntimeException {
        public UnauthorizedException(String msg) {super(msg);}
    }

    public static class ForbiddenException extends RuntimeException {
        public ForbiddenException(String msg) {super(msg);}
    }
}
