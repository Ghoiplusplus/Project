package code_authentication

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"time"

	"math/rand"

	"github.com/gofiber/fiber/v3"
	"github.com/golang-jwt/jwt/v5"
	"github.com/joho/godotenv"
)

type Response_Map struct {
	Token string
	Exp   time.Time
}

var (
	code_map   = make(map[string]Response_Map)
	err        = godotenv.Load("secret.env")
	secret_key = os.Getenv("JWT_SECRET")
)

func NewCode(token string) string {
	rand.Seed(time.Now().UnixNano())
	code := strconv.Itoa(rand.Intn(100000))
	response := Response_Map{
		Token: token,
		Exp:   time.Now().Add(1 * time.Minute),
	}
	code_map[code] = response
	return code
}

func Response_Handler(c fiber.Ctx) error {
	code := c.Query("code")
	refresh_token := c.Query("refresh-token")
	if refresh_token == "" {
		return c.SendStatus(401)
	}

	entry, exist := code_map[code]
	if !exist || entry.Exp.Before(time.Now()) {
		return c.SendStatus(401)
	}
	token, err := jwt.Parse(refresh_token, func(token *jwt.Token) (interface{}, error) {
		if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
			return nil, fmt.Errorf("Unexpected signing method: %s", token.Header["alg"])
		}
		return secret_key, nil
	})
	if err != nil {
		log.Println("Подпись не потверждена")
		return c.SendStatus(400)
	}

	if claims, ok := token.Claims.(jwt.MapClaims); ok {
		log.Println(claims["email"], claims["exp"])
		return c.JSON(fiber.Map{
			"code":  code,
			"state": code_map[code],
		})
	} else {
		return c.SendStatus(404)
	}
}
