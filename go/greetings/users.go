package greetings

import (
	"fmt"
)

//用户结构体
type Users struct {
	UserId   int    `db:"user_id"`
	Username string `db:"username"`
	Sex      string `db:"sex"`
	Email    string `db:"email"`
}

func This_user() {
	fmt.Println("this is user mod")
	return
}
