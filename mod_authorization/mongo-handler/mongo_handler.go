package mongo_handler

import (
	"context"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"log"
	"main/classes"
	"main/jwt_handler"
	"math/rand"
	"os"
	"time"

	"github.com/gofiber/fiber/v3"
	"go.mongodb.org/mongo-driver/v2/bson"
	"go.mongodb.org/mongo-driver/v2/mongo"
	"go.mongodb.org/mongo-driver/v2/mongo/options"
)

var (
	ClientDB, _ = mongo.Connect(options.Client().ApplyURI(os.Getenv("MONGO_URI")))
	Collection  = ClientDB.Database("userdb").Collection("users")
)

type User struct {
	Name          string `bson:"name"`
	Email         string `bson:"email"`
	Role          string `bson:"permissions"` //Студент, преподаватель, админ
	Refresh_Token string `bson:"refresh_token"`
}

var (
	user       User
	Projection = bson.D{
		{"_id", 0},
		{"email", 1},
		{"permissions", 1},
	}
)

func NewUser(email string) (payload classes.Payload, status string) {
	rand.Seed(time.Now().UnixNano())
	someid := rand.Intn(100000)
	user := User{
		Name:          fmt.Sprintf("Anon %d", someid),
		Email:         email,
		Role:          "Student",
		Refresh_Token: "",
	}
	log.Println(user)

	if err := Collection.FindOne(context.TODO(), bson.D{{"email", email}}, options.FindOne().SetProjection(Projection)).Decode(&payload); err == nil {
		return payload, "Пользователь с такой почтой уже есть"
	} else if _, err := Collection.InsertOne(context.TODO(), user); err == nil {
		Collection.FindOne(context.TODO(), bson.D{{"email", email}}, options.FindOne().SetProjection(Projection)).Decode(&payload)
		return payload, "Новый пользователь внесен в базу данных"
	} else {
		return payload, "Ошибка при внесении документа"
	}
}

func AddRefrToken(payload_rt string) {
	filter := bson.D{{"refresh_token", bson.D{{"$eq", ""}}}}
	update := bson.D{
		{"$set", bson.D{
			{"refresh_token", payload_rt},
		}},
	}
	if res, err := Collection.UpdateOne(context.TODO(), filter, update); err == nil {
		log.Println(res)
	}
}

func UpdateTokenPair(refr_token string) (access_token string, refresh_token string, status error) {
	user := User{}
	payload_rt := classes.Payload_RT{}
	if err := Collection.FindOne(context.TODO(), bson.D{{"refresh_token", refr_token}}).Decode(&user); err == nil {
		decoded_token_base64, _ := base64.StdEncoding.DecodeString(user.Refresh_Token)
		if err := json.Unmarshal(decoded_token_base64, &payload_rt); err != nil {
			log.Fatalf("При парсинге произошла ошибка")
		}
		if payload_rt.Exp.Before(time.Now()) {
			status = fiber.ErrUnauthorized
			return access_token, refresh_token, status
		}
		access_token, refresh_token := jwt_handler.CreateJWT_Pair(classes.Payload{
			Email: user.Email,
			Role:  user.Role,
		})
		_, err := Collection.UpdateOne(context.TODO(), bson.D{{"refresh_token", refr_token}}, bson.D{
			{"$set", bson.D{
				{"refresh_token", refresh_token},
			}},
		})
		if err != nil {
			log.Printf("При обновление токена в БД произошла ошибка: %d\n", err)
		}
		status = nil
		return access_token, refresh_token, status
	} else {
		status = fiber.ErrNotFound
		return access_token, refresh_token, status
	}
}
