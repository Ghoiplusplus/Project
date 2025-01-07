package project.webclient.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import project.webclient.model.SessionData;
import project.webclient.service.SessionService;
import project.webclient.utils.CookieUtil;

@Controller
@Tag(name = "Login Controller", description = "Контроллер для входа в систему (GitHub, Яндекс, Код)")
public class LoginController {

    private final SessionService sessionService;

    public LoginController(SessionService sessionService) {
        this.sessionService = sessionService;
    }

    @Operation(
            summary = "Обработчик логина",
            description = """
                        Если указать параметр type (github, yandex, code), то генерируем новый loginToken, 
                        сохраняем статус ANONYMOUS и перенаправляем на AuthModule. 
                        Если параметр type не указан, пытаемся проверить текущее состояние loginToken.
                    """
    )
    @GetMapping("/login")
    public String loginHandler(
            @Parameter(description = "Тип авторизации: github, yandex, code", example = "github")
            @RequestParam(value = "type", required = false) String type,
            HttpServletRequest request,
            HttpServletResponse response,
            Model model
    ) {
        String sessionToken = CookieUtil.getOrCreateSessionToken(request, response);
        SessionData data = sessionService.getOrCreateSession(sessionToken);

        // Если уже авторизован – редирект на главную
        if ("AUTHORIZED".equalsIgnoreCase(data.getUserState())) {
            return "redirect:/";
        }

        // Если нет параметра type — редирект на главную (или проверяем токен входа)
        if (type == null || type.isEmpty()) {
            // Проверяем loginToken
            sessionService.checkLoginTokenAndUpdateSession(sessionToken);
            data = sessionService.getOrCreateSession(sessionToken);
            if ("AUTHORIZED".equalsIgnoreCase(data.getUserState())) {
                return "redirect:/";
            } else {
                // удаляем сессию и редиректим на главную (как в ТЗ)
                sessionService.removeSession(sessionToken);
                CookieUtil.deleteSessionCookie(response);
                return "redirect:/";
            }
        } else {
            // type задан (github, yandex, code)
            // генерируем новый токен сессии и токен входа
            String authUrl = sessionService.startLogin(sessionToken, type);
            // получили URL от AuthModuleClient. Редиректим туда
            // или можем вывести ссылку на страницу, откуда user сам кликнет
            model.addAttribute("authUrl", authUrl);
            return "redirect:" + authUrl;
        }
    }
}
