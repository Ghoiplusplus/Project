package authorization_handler

import (
	"github.com/gofiber/fiber/v3"
	"golang.org/x/oauth2"
	"golang.org/x/oauth2/github"
)

func Github(c *fiber.Ctx) {
	var (
		conf = &oauth2.Config{
			ClientID:     "Ov23liCoYYpUql2puepQ",
			ClientSecret: "",
			Scopes:       []string{"user:email"},
			Endpoint:     github.Endpoint,
		}
		token = "" // формируется веб/тг клиентом
	)
}
