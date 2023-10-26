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
	db, err := sqlx.Open("mysql", "root:123456@tcp(192.168.79.132:3310)/testdb")
	//database, err := sqlx.Open("数据库类型", "用户名:密码@tcp(地址:端口)/数据库名")
	if err != nil {
		fmt.Println("open mysql failed: ", err)
		return
	} else {
		fmt.Println("open mysql success")
	}

	fmt.Println("db:", db)
	r, err := db.Exec("create database if not exists testdb ")
	if err != nil {
		fmt.Println("create database testdb failed: ", err)
		return
	} else {
		fmt.Println("create database testdb success")
	}

	r1, err := db.Exec("create table if not exists user(username varchar(32),sex char(16),email char(32));")
	if err != nil {
		fmt.Println("create table user failed: ", err)
		return
	} else {
		fmt.Println("create table user success:", r1)
	}

	sql := "insert into user(username,sex, email)values (?,?,?)"
	value := [3]string{"user01", "man", "user01@163.com"}

	//执行SQL语句
	r2, err := db.Exec(sql, value[0], value[1], value[2])
	if err != nil {
		fmt.Println("exec failed,", err)
		return
	}
	fmt.Println("affect rows:", r2)

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

	a := []string{"123"}
	fmt.Println("a="+a[len(a)-1])
	// greetings.This_user()
	// test_mysql()

	log.SetPrefix("greetings:")
	log.SetFlags(0)
	// log.Fatal("this is log test")
	msg, err := greetings.Hello("myname")
	if err != nil {
		fmt.Println("----------error-----------")
		log.Fatal(err)
		return
	}

	fmt.Println(msg)

	// A slice of names.
	names := []string{"Gladys", "Samantha", "Darrin"}

	// Request greeting messages for the names.
	messages, err := greetings.Hellos(names)
	if err != nil {
		log.Fatal(err)
	}
	// If no error was returned, print the returned map of
	// messages to the console.
	fmt.Println(messages)

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
