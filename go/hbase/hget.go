package main

import (
	"flag"
	"os"
)

var host = flag.String("host", "localhost:2181", "The location where HBase is running")

func main() {
	flag.Parse()
	TestTableOpMain(false)
	TestGetMain()
	TestPutMain()
	TestDeleteMain()
	os.Exit(0)
}
