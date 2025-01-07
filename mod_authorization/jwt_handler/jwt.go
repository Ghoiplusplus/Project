package jwt_handler

import (
	"encoding/base64"
	"encoding/json"
	"log"
	mongo_handler "main/mongo-handler"
	"os"
	"time"

	_ "github.com/gofiber/contrib/jwt"
	"github.com/golang-jwt/jwt/v5"
	"github.com/joho/godotenv"
)

type Payload_RT struct {
	Email string    `json:"email"`
	Exp   time.Time `json:"exp"`
}

var (
	err    = godotenv.Load("secret.env")
	secret = []byte(os.Getenv("JWT_SECRET"))
)

func CreateJWT_Pair(payload_in mongo_handler.Payload) (string, string) {
	var (
		Payload_AT = jwt.MapClaims{
			"role": payload_in.Role,
			"exp":  time.Now().Add(1 * time.Minute),
		}
		Payload_RT = Payload_RT{
			Email: payload_in.Email,
			Exp:   time.Now().Add(7 * 24 * time.Hour),
		}
	)

	json, _ := json.Marshal(Payload_RT)
	str := base64.StdEncoding.EncodeToString(json)
	log.Println(secret)

	access_token := jwt.NewWithClaims(jwt.SigningMethodHS256, Payload_AT)
	access_token_string, err := access_token.SignedString(secret)
	log.Println(err)
	return access_token_string, str
}
