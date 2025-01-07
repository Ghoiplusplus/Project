package project.webclient.model;

import lombok.Data;

@Data
public class Tokens {
    private String accessToken;
    private String refreshToken;
}
