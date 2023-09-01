package main

import (
	"flag"
	"os"
)

var host = flag.String("host", "192.168.79.132:12181", "The location where HBase is running")

func main() {
	flag.Parse()
	InitHbaseClient()
	InitHbaseAdminClient()
	// TestTableOpMain(false)
	TestCreateTable("student", []string{"cf"}, 3)
	TestPutOneRow("student", "row1", "cf", "c1", "value 1")
	TestPutOneRow("student", "row1", "cf", "c1", "value 2")
	TestPutOneRow("student", "row1", "cf", "c1", "value 3")
	go TestCheckAndPut("student", "row1", "cf", "c1", "value 3", "new value 3")
	go TestCheckAndPut("student", "row1", "cf", "c1", "value 3", "new value 4")
	TestGetMain("student", "row1")

	// TestPutMain("student", "row1", "cf", "c2", "value 111")
	// TestDeleteMain()
	// TestDeleteTable("student")
	os.Exit(0)
}
