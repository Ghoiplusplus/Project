package main

import (
	"context"
	"fmt"
	_ "go/token"
	"log"
	"main/builder"
	"main/classes"
	"main/code_authentication"
	"main/jwt_handler"
	mongoDB "main/mongo-handler"

	"time"

	"github.com/gofiber/fiber/v2/utils"
	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/fiber/v3/client"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"go.mongodb.org/mongo-driver/mongo/readpref"
	"golang.org/x/oauth2"
)

var (
	app           = fiber.New(fiber.Config{})
	response_map  = make(map[string]*classes.Pending_Request)
	oauthConfig_G = classes.OAuth2_conf_github()
	oauthConfig_Y = classes.OAuth2_conf_yandex()
	clientDB, _   = mongo.Connect(context.TODO(), options.Client().ApplyURI("mongodb://localhost:27017/"))
	_             = clientDB.Database("userdb").Collection("users")
	ctx, _        = context.WithTimeout(context.Background(), 2*time.Second)
)

func main() {

	log.Println(clientDB.Ping(ctx, readpref.Primary()))

	builder.Buildpage(app)

	// приходит запрос авторизации ()

	app.Get("/login", func(c fiber.Ctx) error {
		r := classes.NewR()
		if err := c.Bind().Header(&r); err != nil {
			return err
		}

		if c.Query("token") != "" {
			r.Token = utils.CopyString(c.Query("token"))
		} else {
			return c.SendString("Токен отсутсвует")
		}

		response_map[r.Token] = classes.NewPR()
		log.Println(r.Token)

		if r.Type == "github" || c.Query("type") == "github" {
			url := oauthConfig_G.AuthCodeURL(r.Token, oauth2.AccessTypeOnline)
			return c.Redirect().To(url)
		}
		if r.Type == "yandex" || c.Query("type") == "yandex" {
			url := oauthConfig_Y.AuthCodeURL(r.Token, oauth2.AccessTypeOnline)
			return c.Redirect().To(url)
		}
		if r.Type == "code" || c.Query("type") == "code" {
			return c.Redirect().To("/code_cb?state=" + c.Query("type"))
		}
		return c.SendStatus(404)
	})

	// обработка ответа от github

	app.Get("/github_oauth_cb", func(c fiber.Ctx) error {
		entry, exist := response_map[c.Query("state")]
		if !exist || response_map[c.Query("state")].Expires_in.Before(time.Now()) {
			fmt.Printf("Такого токена не существует или его срок действия истек")
			c.Redirect().To("/")
			return c.SendStatus(401)
		}
		if c.Query("error") == "access_denied" {
			entry.Status.Recevied = true
			entry.Status.Access_Denied = true
			c.Redirect().To("/")
			return c.SendStatus(401)
		}
		entry.Status.Recevied = true

		code := c.Query("code")
		token, err := oauthConfig_G.Exchange(oauth2.NoContext, code)
		if err != nil {
			return c.SendStatus(403)
		}
		log.Println(token)

		// запрос данных к API
		cc := client.New()
		cc.SetTimeout(10 * time.Second)
		clientCfg := make(map[string]string)
		clientCfg["Authorization"] = "Bearer " + token.AccessToken
		clientCfg["Content-Type"] = "application/json"
		respone, err2 := cc.Get("https://api.github.com/user/emails", client.Config{
			Header: clientCfg,
		})
		if err2 != nil {
			log.Printf("Произошла ошибка при запросе данных: %s\n", err2)
			return c.SendStatus(403)
		}

		var (
			resp []classes.Response_Github
		)

		if err = respone.JSON(&resp); err != nil {
			panic(err)
		}

		payload, status := mongoDB.NewUser(resp[0].Email)

		jwt, refresh_token := jwt_handler.CreateJWT_Pair(payload)

		log.Println(status)
		log.Println(jwt)

		mongoDB.AddRefrToken(refresh_token)

		if entry, exist := response_map[c.Query("state")]; exist {
			entry.Status.Access = true
			entry.Status.Access_token = jwt
			entry.Status.Refresh_token = refresh_token
		}

		const javaCallbackURL = "http://localhost:10800/auth/callback"

		return c.Redirect().To(fmt.Sprintf("%s?state=%s", javaCallbackURL, c.Query("state")))
	})

	// обработка ответа от яндекса

	app.Get("/yandex_oauth_cb", func(c fiber.Ctx) error {
		entry, exist := response_map[c.Query("state")]
		if !exist || response_map[c.Query("state")].Expires_in.Before(time.Now()) {
			fmt.Printf("Такого токена не существует или его срок действия истек")
			c.Redirect().To("http://127.0.0.1:2228/login/response?state=" + c.Query("state"))
			return c.SendStatus(401)
		}
		if c.Query("error") == "access_denied" {
			entry.Status.Access_Denied = true
			c.Redirect().To("http://127.0.0.1:2228/login/response?state=" + c.Query("state"))
			return c.SendStatus(401)
		}
		entry.Status.Recevied = true

		code := c.Query("code")
		token, err := oauthConfig_Y.Exchange(oauth2.NoContext, code)
		if err != nil {
			return c.SendStatus(403)
		}
		log.Println(token)

		// запрос данных к API
		cc := client.New()
		cc.SetTimeout(10 * time.Second)
		clientCfg := make(map[string]string, 2)
		clientCfg["Authorization"] = "OAuth " + token.AccessToken
		clientCfg["format"] = "json"
		respone, err2 := cc.Get("https://login.yandex.ru/info", client.Config{
			Header: clientCfg,
		})
		if err2 != nil {
			log.Printf("Произошла ошибка при запросе данных: %s\n", err2)
			return c.SendStatus(403)
		}

		var (
			resp classes.Response_Yandex
		)

		if err := respone.JSON(&resp); err != nil {
			panic(err)
		}
		payload, status := mongoDB.NewUser(resp.Emails[0])

		jwt, refresh_token := jwt_handler.CreateJWT_Pair(payload)

		log.Println(status)
		log.Println(jwt)

		mongoDB.AddRefrToken(refresh_token)

		if entry, exist := response_map[c.Query("state")]; exist {
			entry.Status.Access = true
			entry.Status.Access_token = jwt
			entry.Status.Refresh_token = refresh_token
		}

		const javaCallbackURL = "http://localhost:10800/auth/callback"

		return c.Redirect().To(fmt.Sprintf("%s?state=%s", javaCallbackURL, c.Query("state")))
	})

	app.Get("/code_cb", func(c fiber.Ctx) error {
		code := code_authentication.NewCode(c.Query("state"))
		return c.SendString(code)
	})

	app.Get("/response", response)

	app.Listen(":2228")
}

func response(b fiber.Ctx) error {
	if _, exist := response_map[b.Query("state")]; !exist {
		return b.SendStatus(404)
	}

	entry := response_map[b.Query("state")]
	if !entry.Status.Recevied {
		b.Redirect().To("/")
		return b.SendStatus(102)
	}

	if entry.Status.Access_Denied {
		return b.SendStatus(403)
	}

	if entry.Status.Access {
		return b.JSON(fiber.Map{
			"Access Token":  entry.Status.Access_token,
			"Refresh Token": entry.Status.Refresh_token,
		}, "application/problem+json")
	}
	return b.SendStatus(102)
}
