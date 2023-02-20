package main

import (
	_ "database/sql"
	"fmt"

	a "github.com/ibmdb/go_ibm_db"
)

func exam3_main() {
	con := "HOSTNAME=192.168.79.132;DATABASE=testdb;PORT=50000;UID=db2inst1;PWD=123"
	pool := a.Pconnect("PoolSize=100")

	// SetConnMaxLifetime will take the value in SECONDS
	db := pool.Open(con, "SetConnMaxLifetime=30")
	st, err := db.Prepare("Insert into SAMPLE values('hi','hi','hi','hi')")
	if err != nil {
		fmt.Println(err)
	}
	st.Query()

	// Here the time out is default.
	db1 := pool.Open(con)
	st1, err := db1.Prepare("Insert into SAMPLE values('hi1','hi1','hi1','hi1')")
	if err != nil {
		fmt.Println(err)
	}
	st1.Query()

	db1.Close()
	db.Close()
	pool.Release()
	fmt.Println("success")
}
