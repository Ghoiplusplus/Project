package project.webclient.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class AuthResponse {

    @JsonProperty("Access Token")
    private String accessToken;

    @JsonProperty("Refresh Token")
    private String refreshToken;
}
