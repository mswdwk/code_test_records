package main

import (
	"flag"
	"os"
)

var host = flag.String("host", "localhost:2181", "The location where HBase is running")

func main() {
	flag.Parse()
	InitHbaseClient()
	InitHbaseAdminClient()
	// TestTableOpMain(false)
	TestCreateTable("student", []string{"cf"}, 3)
	TestPutOneRow("student", "row1", "cf", "c1", "value 1")
	TestPutOneRow("student", "row1", "cf", "c1", "value 2")
	TestPutOneRow("student", "row1", "cf", "c1", "value 3")
	TestPutOneRow("student", "row2", "cf", "c1", "value 3")
	TestPutOneRow("student", "row", "cf", "c1", "value row")
	TestPutOneRow("student", "row12", "cf", "c1", "value row12")
	TestPutOneRow("student", "row13", "cf", "c1", "value row13")
	TestPutOneRow("student", "row11", "cf", "c1", "value row11")
	TestPutOneRow("student", "row1a", "cf", "c1", "value row1a")
	go TestCheckAndPut("student", "row1", "cf", "c1", "value 3", "new value 3")
	go TestCheckAndPut("student", "row1", "cf", "c1", "value 3", "new value 4")
	TestGetMain("student", "row1")
	TableScanRange("student", "row1", "row2")
	TableScanRange("student", "row1", "row3")
	TableScanRange("student", "ro", "row3")
	TableScanRange("student", "", "row3")
	TableScanRange("student", "row1", "")

	// TestPutMain("student", "row1", "cf", "c2", "value 111")
	// TestDeleteMain()
	// TestDeleteTable("student")
	os.Exit(0)
}
