package main

import (
	"bufio"
	"fmt"
	"io"
	"os"

	"github.com/pingcap/tidb/parser"

	_ "github.com/pingcap/tidb/parser/test_driver"
)

var (
	error_sql_filename string   = "error.sql"
	error_count        int      = 0
	error_fp           *os.File = nil
)

func ReadFile2(path string) error {
	fileHanle, err := os.OpenFile(path, os.O_RDONLY, 0666)
	if err != nil {
		fmt.Printf("open file %s failed\n", path)
		return err
	}
	defer fileHanle.Close()

	reader := bufio.NewReader(fileHanle)

	var count = 0
	// 按行处理txt
	for {
		count += 1
		line, _, err := reader.ReadLine()
		if err == io.EOF {
			break
		}

		if false == parse_one_sql(string(line)) {
			error_count++
			record_error_sql(line)
		}
		fmt.Printf("count %04d error_count %04d\r", count, error_count)
	}
	fmt.Printf("\nend of the file \n")
	return nil
}

func record_error_sql(sql []byte) error {
	var err error = nil
	if nil == error_fp {
		error_fp, err = os.OpenFile(error_sql_filename, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0666)
	}
	if nil != err {
		fmt.Printf("open error record file failed %s\n", error_sql_filename)
		return err
	}
	error_fp.Write(append(sql, '\n'))
	return nil
}

func parse_one_sql(sql string) bool {
	p := parser.New()
	_, _, err := p.Parse(sql, "", "")
	if nil != err {
		// fmt.Println("sql error: ", sql)
		return false
	}
	return true
}

func main() {
	// TODO: USE FUNC ARGUMENT. ReadFile2(,parse_one_sql)
	var args = os.Args

	if len(args) < 2 {
		fmt.Println("Usage: program input_sql_file")
		return
	}
	if len(args) > 2 {
		error_sql_filename = args[2]
	}
	fmt.Println(args)
	var filename string = args[1]
	fmt.Println("input file name is ", filename)
	ReadFile2(filename)
	if error_count > 0 {

	}
}
