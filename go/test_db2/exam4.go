package main

import (
	"database/sql"
	"fmt"
	"time"

	a "github.com/ibmdb/go_ibm_db"
)

func ExecQuery(st *sql.Stmt) error {
	res, err := st.Query()
	if err != nil {
		fmt.Println(err)
	}
	cols, _ := res.Columns()

	fmt.Printf("%s    %s   %s     %s\n", cols[0], cols[1], cols[2], cols[3])
	defer res.Close()
	for res.Next() {
		var t, x, m, n string
		err = res.Scan(&t, &x, &m, &n)
		fmt.Printf("%v  %v   %v     %v\n", t, x, m, n)
	}
	return nil
}

func exam4_main() {
	con := "HOSTNAME=192.168.79.132;DATABASE=testdb;PORT=50000;UID=db2inst1;PWD=123"
	pool := a.Pconnect("PoolSize=5")

	ret := pool.Init(5, con)
	if ret != true {
		fmt.Println("Pool initializtion failed")
	}
	fmt.Println("start example 4")
	for i := 0; i < 20; i++ {
		fmt.Println("start pool ", i)
		db1 := pool.Open(con, "SetConnMaxLifetime=10")
		if db1 != nil {
			st1, err1 := db1.Prepare("select * from VMSAMPLE")
			if err1 != nil {
				fmt.Println("err1 : ", err1)
			} else {
				go func() {
					ExecQuery(st1)
					db1.Close()
				}()
			}
		}
	}
	time.Sleep(30 * time.Second)
	pool.Release()
}
