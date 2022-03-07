package main

import (
	"fmt"
	"log"

	"example.com/greetings"
	_ "github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
)

func fibonacci(n int, c chan int) {
	x, y := 0, 1
	for i := 0; i < n; i++ {
		c <- x
		x, y = y, x+y
	}
	close(c)
}

func test_mysql() {
	db, err := sqlx.Open("mysql", "root:123@tcp(192.168.79.133:3310)/testdb")
	//database, err := sqlx.Open("数据库类型", "用户名:密码@tcp(地址:端口)/数据库名")
	if err != nil {
		fmt.Println("open mysql filed: ", err)
		return
	} else {
		fmt.Println("open mysql success")
	}

	sql := "insert into user(username,sex, email)values (?,?,?)"
	value := [3]string{"user01", "man", "user01@163.com"}

	//执行SQL语句
	r, err := db.Exec(sql, value[0], value[1], value[2])
	if err != nil {
		fmt.Println("exec failed,", err)
		return
	}

	//查询最后一天用户ID，判断是否插入成功
	id, err := r.LastInsertId()
	if err != nil {
		fmt.Println("exec failed,", err)
		return
	}
	fmt.Println("insert succ", id)

	//查询最后一天用户ID，判断是否插入成功
	affected_rows, err := r.RowsAffected()
	if err != nil {
		fmt.Println("exec failed,", err)
		return
	}
	fmt.Println("insert succ", affected_rows)
}

func list_dir() {
	// dirname := "C:\\Users\\lenovo\\"
	// greetings.ListFiles(dirname, 0)
}

func main() {

	greetings.This_user()
	test_mysql()

	log.SetPrefix("greetings:")
	log.SetFlags(0)
	msg, err := greetings.Hello("")
	if err != nil {
		fmt.Println("----------error-----------")
		log.Fatal(err)
	}

	fmt.Println(msg)
	c := make(chan int, 10)
	go fibonacci(cap(c), c)
	// range 函数遍历每个从通道接收到的数据，因为 c 在发送完 10 个
	// 数据之后就关闭了通道，所以这里我们 range 函数在接收到 10 个数据
	// 之后就结束了。如果上面的 c 通道不关闭，那么 range 函数就不
	// 会结束，从而在接收第 11 个数据的时候就阻塞了。
	var id = 0
	for i := range c {
		fmt.Println(id, "", i)
		id += 1
	}
	fmt.Println("finish")

}
