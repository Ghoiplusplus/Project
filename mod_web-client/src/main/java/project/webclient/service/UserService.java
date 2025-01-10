package project.webclient.service;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import project.webclient.model.mainmoduleapi.User;
import project.webclient.utils.HttpClientUtil;

import java.util.Arrays;
import java.util.List;

@Service
public class UserService {

    @Value("${cpp.api.base-url}")
    private String baseUrl;

    /**
     * Добавить нового пользователя.
     *
     * @param user Пользователь для добавления.
     */
    public void addUser(User user) {
        try {
            String url = String.format("%s/users", baseUrl);
            HttpClientUtil.sendPostRequest(url, user, Void.class);
        } catch (Exception e) {
            // Обработка ошибки
            e.printStackTrace();
            throw new RuntimeException("Не удалось добавить пользователя.");
        }
    }

    /**
     * Получить список пользователей.
     *
     * @return Список пользователей.
     */
    public List<User> getUsers() {
        try {
            String url = String.format("%s/users", baseUrl);
            User[] users = HttpClientUtil.sendGetRequest(url, User[].class);
            return Arrays.asList(users);
        } catch (Exception e) {
            // Обработка ошибки
            e.printStackTrace();
            return Arrays.asList(); // Возвращаем пустой список в случае ошибки
        }
    }

    /**
     * Получить информацию о пользователе по ID.
     *
     * @param id ID пользователя.
     * @return Информация о пользователе.
     */
    public User getUserById(Long id) {
        try {
            String url = String.format("%s/users/%d/name", baseUrl, id);
            return HttpClientUtil.sendGetRequest(url, User.class);
        } catch (Exception e) {
            // Обработка ошибки
            e.printStackTrace();
            throw new RuntimeException("Не удалось получить информацию о пользователе.");
        }
    }

    /**
     * Обновить имя пользователя.
     *
     * @param id   ID пользователя.
     * @param user Новые данные пользователя.
     */
    public void updateUserName(Long id, User user) {
        try {
            String url = String.format("%s/users/%d/name", baseUrl, id);
            HttpClientUtil.sendPutRequest(url, user);
        } catch (Exception e) {
            // Обработка ошибки
            e.printStackTrace();
            throw new RuntimeException("Не удалось обновить информацию о пользователе.");
        }
    }

    /**
     * Заблокировать пользователя.
     *
     * @param id ID пользователя.
     */
    public void blockUser(Long id) {
        try {
            String url = String.format("%s/users/%d/block", baseUrl, id);
            HttpClientUtil.sendPutRequest(url, null);
        } catch (Exception e) {
            // Обработка ошибки
            e.printStackTrace();
            throw new RuntimeException("Не удалось заблокировать пользователя.");
        }
    }

    /**
     * Разблокировать пользователя.
     *
     * @param id ID пользователя.
     */
    public void unblockUser(Long id) {
        try {
            String url = String.format("%s/users/%d/unblock", baseUrl, id);
            HttpClientUtil.sendPutRequest(url, null);
        } catch (Exception e) {
            // Обработка ошибки
            e.printStackTrace();
            throw new RuntimeException("Не удалось разблокировать пользователя.");
        }
    }

    // Дополнительно: другие методы по необходимости
}