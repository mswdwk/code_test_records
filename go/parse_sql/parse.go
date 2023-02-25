package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"runtime"
	"strings"

	"github.com/pingcap/tidb/parser"

	_ "github.com/pingcap/tidb/parser/test_driver"
)

var (
	success_sql_filename          string   = "success.sql"
	success_sql_normal_filename   string   = ""
	error_sql_filename            string   = "error.sql"
	error_sql_reason_filename     string   = ""
	error_sql_normalized_filename string   = ""
	error_sql_digest_filename     string   = ""
	success_count                 int      = 0
	error_count                   int      = 0
	success_sql_fp                *os.File = nil
	success_sql_normal_fp         *os.File = nil
	error_sql_fp                  *os.File = nil
	error_sql_reason_fp           *os.File = nil
	error_sql_normal_fp           *os.File = nil
	error_sql_digest_fp           *os.File = nil
	ignore_count                  int      = 0
	filenames                              = []string{}
	fps                                    = []**os.File{}
)

func init1() error {
	var err error = nil
	success_sql_normal_filename = success_sql_filename + ".normal"
	error_sql_reason_filename = error_sql_filename + ".reason"
	error_sql_normalized_filename = error_sql_filename + ".normal"
	error_sql_digest_filename = error_sql_filename + ".digest"
	filenames = []string{success_sql_filename, success_sql_normal_filename, error_sql_filename, error_sql_reason_filename, error_sql_normalized_filename, error_sql_digest_filename}
	fps = append(fps, &success_sql_fp, &success_sql_normal_fp, &error_sql_fp, &error_sql_reason_fp, &error_sql_normal_fp, &error_sql_digest_fp)

	for id, filename := range filenames {
		if nil == *fps[id] {
			*fps[id], err = os.OpenFile(filename, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0666)
			fmt.Printf("first open file %s\n", filename)
			if nil != err || nil == fps[id] {
				fmt.Printf("open error record file failed %s fp= %p\n", filename, fps[id])
				return err
			}
		}
	}
	return nil
}

func finish() {
	for _, fp := range fps {
		if nil != *fp {
			(*fp).Close()
		}
	}
}

func ReadFile2(path string) error {
	fileHanle, err := os.OpenFile(path, os.O_RDONLY, 0666)
	if err != nil {
		fmt.Printf("open file %s failed\n", path)
		return err
	}
	defer fileHanle.Close()

	reader := bufio.NewReader(fileHanle)

	var sql_count = 0
	// 按行处理txt
	for {
		line, _, err := reader.ReadLine()
		if err == io.EOF {
			break
		}

		var sql_str = strings.TrimLeft(string(line), " \t\n")
		// fmt.Println("str is ", sql_str, ",index is ", strings.Index(sql_str, "--"))

		if len(sql_str) < 3 || 0 == strings.Index(sql_str, "--") {
			ignore_count++
			continue
		}
		sql_count += 1
		normalized, digest, err := parse_one_sql(sql_str)
		record_one_sql(sql_str, normalized, digest, err)
	}
	fmt.Printf("End of file. Total sql count %04d success_count %04d error_count %04d ignore_count %04d\n", sql_count, success_count, error_count, ignore_count)
	return nil
}

func record_one_sql(sql string, normalized string, digest string, sql_err error) error {
	if len(sql) < 2 {
		return nil
	}
	// TODO: record into database
	if nil != sql_err {
		error_count++
		error_sql_fp.WriteString(sql + "\n")
		error_sql_reason_fp.WriteString(sql_err.Error() + "\n")
		error_sql_normal_fp.WriteString(normalized + "\n")
		error_sql_digest_fp.WriteString(digest + "\n")
	} else {
		success_count++
		success_sql_fp.WriteString(sql + "\n")
		success_sql_normal_fp.WriteString(normalized + "\n")
	}

	return nil
}

func parse_one_sql(sql string) (string, string, error) {
	p := parser.New()
	p.EnableWindowFunc(true)
	// p.SetSQLMode()
	// p.SetParserConfig()
	// p.SetStrictDoubleTypeCheck()
	// p.ParseSQL()
	_, _, err := p.Parse(sql, "", "")

	// if nil != err {
	// 	return "", "", err
	// }
	normalized, digest := parser.NormalizeDigest(sql)
	return normalized, digest.String(), err
}

func main() {
	fmt.Printf("GOMAXPROCS:%d\n", runtime.GOMAXPROCS(0))
	// return
	// TODO: USE FUNC ARGUMENT. ReadFile2(,parse_one_sql)
	var args = os.Args

	if len(args) < 2 {
		fmt.Println("Usage: program input_sql_file [error_sql_filename]")
		return
	}
	if len(args) > 2 {
		error_sql_filename = args[2]
	}

	if err := init1(); nil != err {
		fmt.Println("init failed")
		return
	}

	fmt.Println(args)
	var filename string = args[1]
	fmt.Println("input sql file name is ", filename)
	ReadFile2(filename)

	finish()
}
