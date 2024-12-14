package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/v2/mongo"
	"go.mongodb.org/mongo-driver/v2/mongo/options"
)

func main() {
	client, err := mongo.Connect(options.Client().ApplyURI("mongodb://localhost:27017/"))
	if err != nil {
		log.Fatal(err)
	}
	ctx, cancel := context.WithTimeout(context.Background(), 2*time.Minute)
	defer cancel()

	defer func() {
		if err = client.Disconnect(ctx); err != nil { // не знаю че это, но вроде должно отключать от бд
			panic(err)
		}
	}()
	// тест внесения документа в локальную бд
	collect := client.Database("userdb").Collection("users")
	cur, err := collect.InsertOne(ctx, bson.M{"name": "Pavel", "value": 525252})
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Inserted a single document: ", cur.InsertedID)

}
