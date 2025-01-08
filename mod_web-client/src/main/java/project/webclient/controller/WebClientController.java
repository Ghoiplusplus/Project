package project.webclient.controller;

import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import project.webclient.model.AuthResponse;
import project.webclient.model.SessionData;
import project.webclient.model.UserState;
import project.webclient.service.AuthorizationService;
import project.webclient.service.SessionService;

import java.util.Optional;
import java.util.UUID;

@Controller
public class WebClientController {
    private final SessionService sessionService;
    private final AuthorizationService authorizationService;

    @Value("${session.cookie.name}")
    private String sessionCookieName;

    @Value("${services.auth-module}")
    private String goBackendUrl;

    public WebClientController(SessionService sessionService, AuthorizationService authorizationService) {
        this.sessionService = sessionService;
        this.authorizationService = authorizationService;
    }

    @GetMapping("/")
    public String home(@CookieValue(value = "${session.cookie.name}", defaultValue = "") String sessionToken, Model model, HttpServletResponse response) {
        Optional<SessionData> sessionOpt = sessionService.getSession(sessionToken);

        if (sessionOpt.isEmpty()) {
            // Unknown user
            model.addAttribute("state", "UNKNOWN");
            return "login"; // Show login options
        }

        SessionData session = sessionOpt.get();
        switch (session.getState()) {
            case ANONYMOUS:
                model.addAttribute("state", "ANONYMOUS");
                return "anonymous"; // Show anonymous page or proceed accordingly
            case AUTHORIZED:
                model.addAttribute("state", "AUTHORIZED");
                model.addAttribute("accessToken", session.getAccessToken());
                model.addAttribute("refreshToken", session.getRefreshToken());
                return "home"; // Show authorized home page
            default:
                // Should not reach here
                return "login";
        }
    }

    @GetMapping("/login")
    public String login(@RequestParam(required = false) String type,
                        @CookieValue(value = "${session.cookie.name}", defaultValue = "") String sessionToken,
                        HttpServletResponse response,
                        Model model) {
        if (!StringUtils.hasText(type)) {
            // Redirect to home
            return "redirect:/";
        }

        // Generate new login token
        String loginToken = UUID.randomUUID().toString();

        if (StringUtils.hasText(sessionToken)) {
            // Existing session: Anonymous user
            Optional<SessionData> sessionOpt = sessionService.getSession(sessionToken);
            if (sessionOpt.isPresent() && sessionOpt.get().getState() == UserState.ANONYMOUS) {
                SessionData sessionData = sessionOpt.get();
                sessionData.setLoginToken(loginToken);
                sessionService.updateSession(sessionToken, sessionData);
            } else {
                // Invalid session, treat as Unknown
                sessionService.deleteSession(sessionToken);
                return "redirect:/";
            }
        } else {
            // No session: Generate new session
            String newSessionToken = UUID.randomUUID().toString();
            sessionService.createSession(newSessionToken, UserState.ANONYMOUS, loginToken);

            // Set session token as cookie
            Cookie cookie = new Cookie(sessionCookieName, newSessionToken);
            cookie.setPath("/");
            cookie.setHttpOnly(true);
            cookie.setSecure(true); // Ensure HTTPS in production
            response.addCookie(cookie);
        }

        // Redirect to Authorization module with loginToken
        String goAuthUrl = String.format("%s/login?token=%s&type=%s",
                goBackendUrl,
                loginToken,
                type.toLowerCase());

        return "redirect:" + goAuthUrl;
    }

    @GetMapping("/auth/callback")
    public String authCallback(@RequestParam String state, Model model, HttpServletResponse response,
                               @CookieValue(value = "${session.cookie.name}", defaultValue = "") String sessionToken) {
        if (!StringUtils.hasText(sessionToken)) {
            // No session, redirect to home
            return "redirect:/";
        }

        Optional<SessionData> sessionOpt = sessionService.getSession(sessionToken);
        if (sessionOpt.isEmpty() || sessionOpt.get().getState() != UserState.ANONYMOUS) {
            // Invalid session or state
            sessionService.deleteSession(sessionToken);
            return "redirect:/";
        }

        // Validate login token with Authorization module
        Optional<AuthResponse> authResponseOpt = authorizationService.validateLoginToken(state);
        if (authResponseOpt.isEmpty()) {
            // Invalid or expired token, delete session
            sessionService.deleteSession(sessionToken);
            return "redirect:/";
        }

        AuthResponse authResponse = authResponseOpt.get();
        if (authResponse.getAccessToken() == null || authResponse.getRefreshToken() == null) {
            // Access denied or incomplete tokens
            sessionService.deleteSession(sessionToken);
            return "redirect:/";
        }

        // Update session to Authorized
        SessionData sessionData = sessionOpt.get();
        sessionData.setState(UserState.AUTHORIZED);
        sessionData.setAccessToken(authResponse.getAccessToken());
        sessionData.setRefreshToken(authResponse.getRefreshToken());
        sessionData.setLoginToken(null); // Remove login token
        sessionService.updateSession(sessionToken, sessionData);

        // Redirect to home
        return "redirect:/";
    }

    @GetMapping("/response")
    public String responseHandler(@RequestParam String state, Model model,
                                  @CookieValue(value = "${session.cookie.name}", defaultValue = "") String sessionToken) {
        // This endpoint can be used to handle specific responses if needed
        // For simplicity, it's handled in /auth/callback
        return "redirect:/auth/callback?state=" + state;
    }

    @GetMapping("/logout")
    public String logout(@CookieValue(value = "${session.cookie.name}", defaultValue = "") String sessionToken,
                         HttpServletResponse response) {
        if (StringUtils.hasText(sessionToken)) {
            sessionService.deleteSession(sessionToken);

            // Remove cookie
            Cookie cookie = new Cookie(sessionCookieName, "");
            cookie.setPath("/");
            cookie.setHttpOnly(true);
            cookie.setSecure(true);
            cookie.setMaxAge(0);
            response.addCookie(cookie);
        }

        return "redirect:/";
    }
}
