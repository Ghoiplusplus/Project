package project.webclient.utils;

import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import java.util.UUID;

public class CookieUtil {

    public static final String SESSION_COOKIE_NAME = "SESSION_TOKEN";

    public static String getOrCreateSessionToken(HttpServletRequest request, HttpServletResponse response) {
        // Ищем куку
        Cookie[] cookies = request.getCookies();
        if (cookies != null) {
            for (Cookie c : cookies) {
                if (SESSION_COOKIE_NAME.equals(c.getName()) && c.getValue() != null && !c.getValue().isEmpty()) {
                    return c.getValue();
                }
            }
        }
        // Не нашли – генерируем новый
        String newToken = UUID.randomUUID().toString();
        Cookie cookie = new Cookie(SESSION_COOKIE_NAME, newToken);
        cookie.setHttpOnly(true);
        cookie.setPath("/");
        // время жизни куки на ваше усмотрение, например session only:
        // cookie.setMaxAge(-1);
        response.addCookie(cookie);
        return newToken;
    }

    public static void deleteSessionCookie(HttpServletResponse response) {
        Cookie cookie = new Cookie(SESSION_COOKIE_NAME, "");
        cookie.setPath("/");
        cookie.setMaxAge(0);
        response.addCookie(cookie);
    }
}
