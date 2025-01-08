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
    private UserState state;
    private String loginToken; // Token for initiating login
    private String accessToken; // JWT Access Token
    private String refreshToken; // JWT Refresh Token

    public SessionData(UserState state, String loginToken) {
        this.state = state;
        this.loginToken = loginToken;
    }
}
