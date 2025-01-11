package mongo_handler

import (
	"context"
	"fmt"
	"log"
	"math/rand"
	"os"
	"time"

	"go.mongodb.org/mongo-driver/v2/bson"
	"go.mongodb.org/mongo-driver/v2/mongo"
	"go.mongodb.org/mongo-driver/v2/mongo/options"
)

var (
	ClientDB, _ = mongo.Connect(options.Client().ApplyURI(os.Getenv("MONGO_URI")))
	Collection  = ClientDB.Database("userdb").Collection("users")
)

type User struct {
	Name           string `bson:"name"`
	Email          string `bson:"email"`
	Role           string `bson:"permissions"` //Студент, преподаватель, админ TODO: сделать список разрешений для каждой роли
	Refresh_Tokens string `bson:"refresh_token"`
}

type Payload struct {
	Email string `bson:"email"`
	Role  string `bson:"permissions"`
}

var (
	user       User
	Projection = bson.D{
		{"_id", 0},
		{"email", 1},
		{"permissions", 1},
	}
)

func NewUser(email string) (payload Payload, status string) {
	rand.Seed(time.Now().UnixNano())
	someid := rand.Intn(100000)
	user := User{
		Name:           fmt.Sprintf("Anon %d", someid),
		Email:          email,
		Role:           "Student",
		Refresh_Tokens: "",
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

func UpdateRefrToken(refr_token string) {

}
