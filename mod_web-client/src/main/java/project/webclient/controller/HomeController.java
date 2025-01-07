package project.webclient.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import project.webclient.model.SessionData;
import project.webclient.service.SessionService;
import project.webclient.utils.CookieUtil;

@Controller
@RequestMapping("/")
@Tag(name = "Home Controller", description = "Контроллер для главной страницы и статусов пользователя")
public class HomeController {

    private final SessionService sessionService;

    public HomeController(SessionService sessionService) {
        this.sessionService = sessionService;
    }

    @Operation(
            summary = "Главная страница",
            description = """
                        Определяет статус пользователя (UNKNOWN, ANONYMOUS или AUTHORIZED) 
                        и возвращает соответствующий шаблон (index.html, waiting.html, cabinet.html и т.д.).
                    """
    )
    @GetMapping("/")
    public String homePage(HttpServletRequest request, HttpServletResponse response, Model model) {
        String sessionToken = CookieUtil.getOrCreateSessionToken(request, response);
        SessionData data = sessionService.getOrCreateSession(sessionToken);

        switch (data.getUserState()) {
            case "UNKNOWN":
                // Страница, предлагающая авторизоваться
                model.addAttribute("message", "Вы не авторизованы. Пожалуйста, войдите через GitHub, Яндекс ID или Код.");
                return "index"; // index.html с кнопками "Войти"
            case "ANONYMOUS":
                // Попробуем проверить, не «созрел» ли логин
                sessionService.checkLoginTokenAndUpdateSession(sessionToken);
                // Снова перечитаем из Redis
                data = sessionService.getOrCreateSession(sessionToken);
                if ("ANONYMOUS".equalsIgnoreCase(data.getUserState())) {
                    // Ещё не авторизовался
                    model.addAttribute("message", "Вы находитесь в процессе авторизации. Подтвердите вход или подождите.");
                    return "waiting";
                } else if ("AUTHORIZED".equalsIgnoreCase(data.getUserState())) {
                    // уже стало AUTHORIZED
                    // покажем личный кабинет
                    return "redirect:/";
                }
                // иначе (UNKNOWN) – редирект на главную
                return "redirect:/";
            case "AUTHORIZED":
                // Показываем личный кабинет пользователя
                model.addAttribute("message", "Добро пожаловать! Ваши личные данные...");
                return "cabinet"; // cabinet.html
            default:
                // что-то странное
                return "index";
        }
    }
}
