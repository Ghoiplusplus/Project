package mongo_handler

import (
	"context"
	"fmt"
	"log"
	"math/rand"
	"time"
	"os"

	"go.mongodb.org/mongo-driver/v2/bson"
	"go.mongodb.org/mongo-driver/v2/mongo"
	"go.mongodb.org/mongo-driver/v2/mongo/options"
)

var (
	ClientDB, _ = mongo.Connect(options.Client().ApplyURI(os.Getenv("MONGO_URI")))
	Collection  = ClientDB.Database("userdb").Collection("users")
)

type User struct {
	Name           string   `bson:"name"`
	Email          string   `bson:"email"`
	Role           string   `bson:"role"` //Студент, преподаватель, админ TODO: сделать список разрешений для каждой роли
	Refresh_Tokens []string `bson:"refresh_tokens"`
}

type Payload struct {
	Email string `bson:"email"`
	Role  string `bson:"role"`
}

var (
	user       User
	Projection = bson.D{
		{"_id", 0},
		{"email", 1},
		{"role", 1},
	}
)

func NewUser(email string) (payload Payload, status string) {
	rand.Seed(time.Now().UnixNano())
	someid := rand.Intn(100000)
	user.Name = fmt.Sprintf("Anon %d", someid)
	user.Email = email
	user.Role = "student"
	user.Refresh_Tokens = make([]string, 0)

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
	filter := bson.D{{"refresh_tokens", bson.D{{"$size", 0}}}}
	update := bson.D{
		{"$set", bson.D{
			{"refresh_tokens.0", payload_rt},
		}},
	}
	if res, err := Collection.UpdateOne(context.TODO(), filter, update); err == nil {
		log.Println(res)
	}
}
