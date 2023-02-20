package main

import (
	"fmt"
	"log"

	_ "github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
)

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
	test_mysql()
	log.SetPrefix("greetings:")
	log.SetFlags(0)
	fmt.Println("finish")
}
