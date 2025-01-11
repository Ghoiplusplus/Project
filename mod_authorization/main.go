package main

import (
	"main/builder"
	"main/handlers"

	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/fiber/v3/middleware/logger"
)

var (
	app = fiber.New(fiber.Config{})
)

func main() {
	builder.Buildpage(app)

	app.Use(logger.New(logger.Config{
		Format: "[${ip}]:${port} ${status} - ${method} ${path}\n",
	}))

	app.Get("/login", handlers.Login)                     // приходит запрос авторизации
	app.Get("/github_oauth_cb", handlers.Github_oauth_cb) // обработка ответа от github
	app.Get("/yandex_oauth_cb", handlers.Yandex_oauth_cb) // обработка ответа от яндекса
	app.Get("/code_cb", handlers.Code_cb)                 // обработка запроса авторизации через код
	app.Get("/response/:state", handlers.Response)        // статусы пользователя

	app.Listen(":2228", fiber.ListenConfig{
		EnablePrefork:     true,
		EnablePrintRoutes: true,
	})
}
