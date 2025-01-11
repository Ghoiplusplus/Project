package project.webclient.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import project.webclient.model.mainmoduleapi.User;
import project.webclient.service.UserService;

import java.util.List;

@Controller
@RequestMapping("/users")
public class UserController {

    @Autowired
    private UserService userService;

    // Просмотр списка пользователей
    @GetMapping
    public String listUsers(Model model) {
        List<User> users = userService.getUsers();
        model.addAttribute("users", users);
        return "users/list"; // Шаблон users/list.html
    }

    // Показать форму добавления нового пользователя
    @GetMapping("/add")
    public String showAddUserForm(Model model) {
        model.addAttribute("user", new User());
        return "users/add"; // Шаблон users/add.html
    }

    // Обработка добавления нового пользователя
    @PostMapping("/add")
    public String addUser(@ModelAttribute User user, Model model) {
        userService.addUser(user);
        return "redirect:/users";
    }

    // Просмотр информации о конкретном пользователе
    @GetMapping("/{id}")
    public String viewUser(@PathVariable Long id, Model model) {
        User user = userService.getUserById(id);
        model.addAttribute("user", user);
        return "users/view"; // Шаблон users/view.html
    }

    // Показать форму редактирования пользователя
    @GetMapping("/{id}/edit")
    public String showEditUserForm(@PathVariable Long id, Model model) {
        User user = userService.getUserById(id);
        model.addAttribute("user", user);
        return "users/edit"; // Шаблон users/edit.html
    }

    // Обработка редактирования пользователя
    @PostMapping("/{id}/edit")
    public String editUser(@PathVariable Long id, @ModelAttribute User user, Model model) {
        userService.updateUserName(id, user);
        return "redirect:/users/" + id;
    }

    // Блокировка пользователя
    @PostMapping("/{id}/block")
    public String blockUser(@PathVariable Long id, Model model) {
        userService.blockUser(id);
        return "redirect:/users/" + id;
    }

    // Разблокировка пользователя
    @PostMapping("/{id}/unblock")
    public String unblockUser(@PathVariable Long id, Model model) {
        userService.unblockUser(id);
        return "redirect:/users/" + id;
    }
}