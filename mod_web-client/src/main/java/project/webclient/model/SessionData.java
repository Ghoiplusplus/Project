package project.webclient.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.io.Serializable;

@NoArgsConstructor
@AllArgsConstructor
@Getter
@Setter
public class SessionData implements Serializable {
    private String userState;
    private String loginToken; // Токен входа (используется при проверке Авторизации)
    private String accessToken; // JWT access token
    private String refreshToken; // JWT refresh token
}
