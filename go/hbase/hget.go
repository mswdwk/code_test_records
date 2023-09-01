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
	TestPutMain("student", "row1", "cf", "a", "value 1")
	TestPutMain("student", "row1", "cf", "a", "value 2")
	TestPutMain("student", "row1", "cf", "a", "value 3")
	TestPutMain("student", "row1", "cf", "c2", "value 111")
	TestGetMain("student", "row1")
	// TestDeleteMain()
	// TestDeleteTable("student")
	os.Exit(0)
}
