package jwt_handler

import (
	"encoding/base64"
	"encoding/json"
	"log"
	"main/classes"
	"os"
	"time"

	_ "github.com/gofiber/contrib/jwt"
	"github.com/golang-jwt/jwt/v5"
	"github.com/joho/godotenv"
)

type Permissions struct {
	jwt.RegisteredClaims

	UserListRead      bool `json:"user:list:read,omitempty"`
	UserFullNameWrite bool `json:"user:fullName:write,omitempty"`
	UserDataRead      bool `json:"user:data:read,omitempty"`
	UserRolesRead     bool `json:"user:roles:read,omitempty"`
	UserRolesWrite    bool `json:"user:roles:write,omitempty"`
	UserBlockRead     bool `json:"user:block:read,omitempty"`
	UserBlockWrite    bool `json:"user:block:write,omitempty"`

	// Дисциплина
	CourseInfoWrite bool `json:"course:info:write,omitempty"`
	CourseTestList  bool `json:"course:testList,omitempty"`
	CourseTestRead  bool `json:"course:test:read,omitempty"`
	CourseTestWrite bool `json:"course:test:write,omitempty"`
	CourseTestAdd   bool `json:"course:test:add,omitempty"`
	CourseTestDel   bool `json:"course:test:del,omitempty"`
	CourseUserList  bool `json:"course:userList,omitempty"`
	CourseUserAdd   bool `json:"course:user:add,omitempty"`
	CourseUserDel   bool `json:"course:user:del,omitempty"`
	CourseAdd       bool `json:"course:add,omitempty"`
	CourseDel       bool `json:"course:del,omitempty"`

	// Вопросы
	QuestListRead bool `json:"quest:list:read,omitempty"`
	QuestRead     bool `json:"quest:read,omitempty"`
	QuestUpdate   bool `json:"quest:update,omitempty"`
	QuestCreate   bool `json:"quest:create,omitempty"`
	QuestDel      bool `json:"quest:del,omitempty"`

	// Тесты
	TestQuestDel    bool `json:"test:quest:del,omitempty"`
	TestQuestAdd    bool `json:"test:quest:add,omitempty"`
	TestQuestUpdate bool `json:"test:quest:update,omitempty"`
	TestQuestRead   bool `json:"test:quest:read,omitempty"`
	TestAnswerRead  bool `json:"test:answer:read,omitempty"`

	// Ответы
	AnswerRead   bool `json:"answer:read,omitempty"`
	AnswerUpdate bool `json:"answer:update,omitempty"`
	AnswerDel    bool `json:"answer:del,omitempty"`

	Expire time.Time `json:"exp"`
}

func Permissions_Student() *Permissions {
	student := Permissions{
		RegisteredClaims:  jwt.RegisteredClaims{},
		UserListRead:      true,
		UserFullNameWrite: true,
		UserDataRead:      true,
		UserBlockRead:     true,
		CourseTestList:    true,
		CourseUserList:    true,
		CourseUserAdd:     true,
		CourseUserDel:     true,
		TestAnswerRead:    true,
		QuestListRead:     true,
		QuestRead:         true,
		Expire:            time.Now().Add(1 * time.Minute),
	}
	return &student
}

var (
	err    = godotenv.Load("secret.env")
	secret = []byte(os.Getenv("JWT_SECRET"))
)

func CreateJWT_Pair(payload_in classes.Payload) (string, string) {
	var (
		Payload_AT = Permissions_Student()
		Payload_RT = classes.Payload_RT{
			Email: payload_in.Email,
			Exp:   time.Now().Add(7 * 24 * time.Hour),
		}
	)
	log.Println(Permissions_Student())
	json, _ := json.Marshal(Payload_RT)
	refresh_token := base64.StdEncoding.EncodeToString(json)
	log.Println(secret)

	access_token := jwt.NewWithClaims(jwt.SigningMethodHS256, Payload_AT)
	access_token_string, err := access_token.SignedString(secret)
	if err != nil {
		log.Printf("Произошла ошибка при подписи токена: %s\n", err)
	}
	return access_token_string, refresh_token
}
