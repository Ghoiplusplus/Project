package project.webclient.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import project.webclient.service.SessionService;
import project.webclient.utils.CookieUtil;

@Controller
@Tag(name = "Logout Controller", description = "Контроллер для выхода из системы (локально или на всех устройствах)")
public class LogoutController {

    private final SessionService sessionService;

    public LogoutController(SessionService sessionService) {
        this.sessionService = sessionService;
    }

    @Operation(
            summary = "Выход из системы",
            description = """
            Метод GET /logout. Если ?all=true, то происходит выход со всех устройств (AuthModule),
            иначе локальный выход (удаляем сессию в Redis, очищаем куку).
        """
    )
    @GetMapping("/logout")
    public String logout(
            @Parameter(description = "Признак выхода со всех устройств", example = "true")
            @RequestParam(value = "all", required = false) String all,
            HttpServletRequest request,
            HttpServletResponse response
    ) {
        String sessionToken = CookieUtil.getOrCreateSessionToken(request, response);

        if ("true".equalsIgnoreCase(all)) {
            // Выход со всех устройств
            sessionService.logoutAll(sessionToken);
        } else {
            // Выход только локально
            sessionService.logoutLocal(sessionToken);
        }
        // Удаляем куку
        CookieUtil.deleteSessionCookie(response);
        // Редирект на главную
        return "redirect:/";
    }
}
