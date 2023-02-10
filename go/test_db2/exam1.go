package main

import (
	"database/sql"
	"fmt"

	_ "github.com/ibmdb/go_ibm_db"
)

func main() {
	// con := "HOSTNAME=host;DATABASE=name;PORT=number;UID=username;PWD=password"
	con := "HOSTNAME=192.168.79.132;DATABASE=testdb;PORT=50000;UID=db2inst1;PWD=123"
	db, err := sql.Open("go_ibm_db", con)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("connect DB2 OK！")
	}
	db.Close()
}
