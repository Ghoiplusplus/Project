package builder

import (
	"github.com/gofiber/fiber/v3"
)

func Buildpage(app *fiber.App) {
	app.Get("/", func(c fiber.Ctx) error {
		return c.Render("builder/index.html", fiber.Map{})
	})
}
