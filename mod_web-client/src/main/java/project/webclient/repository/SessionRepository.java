package project.webclient.repository;

import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Repository;
import project.webclient.model.SessionData;

import java.time.Duration;
import java.util.concurrent.TimeUnit;

@Repository
public class SessionRepository {

    private final RedisTemplate<String, Object> redisTemplate;
    // Допустим, время жизни ключа в Redis 1 час
    private static final long SESSION_EXPIRE_SECONDS = 3600;

    public SessionRepository(RedisTemplate<String, Object> redisTemplate) {
        this.redisTemplate = redisTemplate;
    }

    public void saveSession(String sessionToken, SessionData data) {
        redisTemplate.opsForValue().set(sessionToken, data, Duration.ofSeconds(SESSION_EXPIRE_SECONDS));
    }

    public SessionData getSession(String sessionToken) {
        Object data = redisTemplate.opsForValue().get(sessionToken);
        if (data instanceof SessionData) {
            return (SessionData) data;
        }
        return null;
    }

    public void deleteSession(String sessionToken) {
        redisTemplate.delete(sessionToken);
    }

    public void updateSessionTTL(String sessionToken) {
        redisTemplate.expire(sessionToken, SESSION_EXPIRE_SECONDS, TimeUnit.SECONDS);
    }
}
