package project.webclient.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import project.webclient.service.SessionService;
import project.webclient.utils.CookieUtil;

@Controller
@Tag(name = "Action Controller", description = "Контроллер для выполнения действий в Главном модуле")
public class ActionController {

    private final SessionService sessionService;

    public ActionController(SessionService sessionService) {
        this.sessionService = sessionService;
    }

    /**
     * Пример: /action/foo -> вызвать в Главном модуле GET /foo
     */
    @GetMapping("/action/{actionPath}")
    @Operation(
            summary = "Выполнить действие",
            description = """
            Выполняет запрос в Главный модуль по пути /{actionPath}, передавая accessToken. 
            При 401 пытается рефрешить токен, при 403 — возвращает сообщение 'нет прав'.
        """
    )
    public String doAction(
            @Parameter(description = "Путь действия (например, courseList, userList)", example = "courseList")
            @PathVariable("actionPath") String actionPath,
            HttpServletRequest request,
            HttpServletResponse response
    ) {
        String sessionToken = CookieUtil.getOrCreateSessionToken(request, response);
        // Пытаемся выполнить действие
        String result = sessionService.doAction(sessionToken, actionPath);

        if (result.startsWith("redirect:")) {
            // если сервис вернул "redirect:/", значит нужно редиректнуть
            return result;
        }
        // иначе рендерим простую текстовую страницу
        // в реальном проекте - передадим результат в модель
        request.setAttribute("result", result);
        return "actionResult"; // некий шаблон, который покажет result
    }
}
