package project.webclient.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import project.webclient.model.SessionData;
import project.webclient.model.UserState;

import java.time.Duration;
import java.util.Optional;

@Service
public class SessionService {
    private static final String SESSION_PREFIX = "session:";

    @Autowired
    private RedisTemplate<String, Object> redisTemplate;

    private static final Duration SESSION_TTL = Duration.ofHours(2);

    /**
     * Create a new session with given session token and initial state.
     *
     * @param sessionToken Unique session token.
     * @param initialState Initial state of the user.
     */
    public void createSession(String sessionToken, UserState initialState, String loginToken) {
        SessionData sessionData = new SessionData(initialState, loginToken);
        redisTemplate.opsForValue().set(sessionToken, sessionData, SESSION_TTL);
    }

    /**
     * Retrieve session data based on session token.
     *
     * @param sessionToken Session token from cookie.
     * @return Optional containing SessionData if session exists.
     */
    public Optional<SessionData> getSession(String sessionToken) {
        Object data = redisTemplate.opsForValue().get(sessionToken);
        if (data instanceof SessionData) {
            return Optional.of((SessionData) data);
        }
        return Optional.empty();
    }

    /**
     * Update session data.
     *
     * @param sessionToken Session token.
     * @param sessionData  New session data.
     */
    public void updateSession(String sessionToken, SessionData sessionData) {
        redisTemplate.opsForValue().set(sessionToken, sessionData, SESSION_TTL);
    }

    /**
     * Delete a session.
     *
     * @param sessionToken Session token to delete.
     */
    public void deleteSession(String sessionToken) {
        redisTemplate.delete(sessionToken);
    }
}
