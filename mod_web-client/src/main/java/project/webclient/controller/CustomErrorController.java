package project.webclient.controller;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.boot.web.servlet.error.ErrorController;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
public class CustomErrorController implements ErrorController {
    @RequestMapping("/error")
    public String handleError(HttpServletRequest request, HttpServletResponse response) {
        int status = response.getStatus();

        if (status == 404) {
            // При 404 — редирект на корень
            return "redirect:/";
        } else {
            // Если это не 404, то отдаём нашу страницу error.html
            // (у вас она может называться иначе)
            return "redirect:/";
        }
    }
}
