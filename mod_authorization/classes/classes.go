package classes

import (
	"os"
	"time"

	"github.com/joho/godotenv"
	"golang.org/x/oauth2"
	"golang.org/x/oauth2/github"
	"golang.org/x/oauth2/yandex"
)

type Response_Github struct {
	Email      string `json:"email"`
	Primary    bool   `json:"primary"`
	Verifed    bool   `json:"verified"`
	Visibility string `json:"visibility"`
}

type Response_Yandex struct {
	Login            string   `json:"login"`
	Old_social_login string   `json:"old_social_login"`
	Default_email    string   `json:"default_email"`
	Id               string   `json:"id"`
	Client_id        string   `json:"client_id"`
	Emails           []string `json:"emails"`
	Psuid            string   `json:"psuid"`
}

type Request struct {
	Authorization string `header:"authorization" json:"authorization"`
	Token         string `header:"state" json:"token"`
	Type          string `header:"type" json:"type"`
}

func NewR() (r Request) {
	r.Authorization = "Basic " // http авторизация (никогда не реализую лол)
	r.Token = ""
	r.Type = ""
	return r
}

type Status struct {
	Recevied      bool
	Access_Denied bool
	Access        bool
	Access_token  string
	Refresh_token string
}

type Pending_Request struct {
	Expires_in time.Time
	Status     Status
}

func NewPR() (pr *Pending_Request) {
	make := Pending_Request{
		Expires_in: time.Now().Add(5 * time.Minute),
		Status: Status{
			Recevied:      false,
			Access_Denied: false,
			Access:        false,
			Access_token:  "",
			Refresh_token: "",
		},
	}
	return &make
}

var (
	err = godotenv.Load("secret.env")
)

func OAuth2_conf_github() (c oauth2.Config) {
	if err != nil {
		panic("Переменные окружения не загружены")
	}
	c.ClientID = os.Getenv("CLIENT_ID_GITHUB")
	c.ClientSecret = os.Getenv("CLIENT_SECRET_GITHUB")
	c.Scopes = []string{"user:email"}
	c.Endpoint = github.Endpoint
	return c
}

func OAuth2_conf_yandex() (c oauth2.Config) {
	if err != nil {
		panic("Переменные окружения не загружены")
	}
	c.ClientID = os.Getenv("CLIENT_ID_YANDEX")
	c.ClientSecret = os.Getenv("CLIENT_SECRET_YANDEX")
	c.Endpoint = yandex.Endpoint
	return c
}

type Payload_RT struct {
	Email string    `json:"email"`
	Exp   time.Time `json:"exp"`
}

type Payload struct {
	Email string `bson:"email"`
	Role  string `bson:"permissions"`
}
