package project.webclient.service;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import project.webclient.model.AuthResponse;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Optional;

@Service
public class AuthorizationService {

    @Value("${services.auth-module}")
    private String goBackendUrl;

    private final HttpClient httpClient;
    private final ObjectMapper objectMapper;

    public AuthorizationService() {
        this.httpClient = HttpClient.newHttpClient();
        this.objectMapper = new ObjectMapper();
    }

    /**
     * Fetches the authentication response from the Go backend using the state token.
     *
     * @param state The state token.
     * @return Optional containing AuthResponse if successful.
     */
    public Optional<AuthResponse> fetchAuthResponse(String state) {
        try {
            String url = String.format("%s/response?state=%s", goBackendUrl, state);
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(url))
                    .GET()
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());

            if (response.statusCode() == 200) {
                AuthResponse authResponse = objectMapper.readValue(response.body(), AuthResponse.class);
                return Optional.of(authResponse);
            } else {
                // Handle non-200 responses
                return Optional.empty();
            }
        } catch (Exception e) {
            // Handle exceptions (e.g., network issues, JSON parsing errors)
            e.printStackTrace();
            return Optional.empty();
        }
    }


    /**
     * Validate login token with Authorization module.
     *
     * @param loginToken Token to validate.
     * @return Validation result.
     */
    public Optional<AuthResponse> validateLoginToken(String loginToken) {
        try {
            String url = String.format("%s/response?state=%s", goBackendUrl, loginToken);
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(url))
                    .GET()
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());

            if (response.statusCode() == 200) {
                AuthResponse authResponse = objectMapper.readValue(response.body(), AuthResponse.class);
                return Optional.of(authResponse);
            } else {
                // Handle invalid or expired tokens
                return Optional.empty();
            }
        } catch (Exception e) {
            e.printStackTrace();
            return Optional.empty();
        }
    }
}
