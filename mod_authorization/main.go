package main

import (
	"main/builder"

	"github.com/gofiber/fiber/v3"
)

func main() {
	app := fiber.New(fiber.Config{})
	builder.Buildpage(app)
	app.Listen(":5252")
}
