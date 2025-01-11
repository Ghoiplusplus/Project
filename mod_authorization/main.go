package main

import (
	"context"
	"log"
	"main/builder"
	"main/handlers"
	"os"

	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/fiber/v3/middleware/logger"
	"go.mongodb.org/mongo-driver/v2/mongo"
	"go.mongodb.org/mongo-driver/v2/mongo/options"
	"go.mongodb.org/mongo-driver/v2/mongo/readpref"
)

var (
	app           = fiber.New(fiber.Config{})
	ClientDB, err = mongo.Connect(options.Client().ApplyURI(os.Getenv("MONGO_URI")))
)

func main() {
	if err := ClientDB.Ping(context.TODO(), readpref.Primary()); err != nil {
		log.Fatalf("БД не отвечает: %d", err)
	}

	builder.Buildpage(app)

	app.Use(logger.New(logger.Config{
		Format: "[${ip}]:${port} ${status} - ${method} ${path}\n",
	}))

	app.Get("/login", handlers.Login)                           // приходит запрос авторизации
	app.Get("/github_oauth_cb", handlers.Github_oauth_cb)       // обработка ответа от github
	app.Get("/yandex_oauth_cb", handlers.Yandex_oauth_cb)       // обработка ответа от яндекса
	app.Get("/code_cb", handlers.Code_cb)                       // обработка запроса авторизации через код
	app.Get("/response/:state", handlers.Response)              // статусы пользователя
	app.Get("/api/newjwtpair/:refr_token", handlers.Newjwtpair) // запрос на выдачу новой пары токенов

	app.Listen(":2228", fiber.ListenConfig{
		EnablePrefork:     true,
		EnablePrintRoutes: true,
	})
}
