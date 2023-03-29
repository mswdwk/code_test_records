package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"
	"sync/atomic"

	"github.com/pingcap/tidb/parser"
	"github.com/pingcap/tidb/parser/ast"

	_ "github.com/pingcap/tidb/parser/test_driver"
)

var (
	success_sql_filename             string   = "success.sql"
	success_sql_normal_filename      string   = ""
	error_sql_filename               string   = "error.sql"
	error_sql_reason_filename        string   = ""
	error_sql_normalized_filename    string   = ""
	error_sql_digest_filename        string   = ""
	success_count                    int      = 0
	error_count                      int      = 0
	parse_sql_rountine_number        *int     = nil
	finish_pasrse_sql_rountine_count int32    = 0
	success_sql_fp                   *os.File = nil
	success_sql_normal_fp            *os.File = nil
	error_sql_fp                     *os.File = nil
	error_sql_reason_fp              *os.File = nil
	error_sql_normal_fp              *os.File = nil
	error_sql_digest_fp              *os.File = nil
	ignore_count                     int      = 0
	filenames                                 = []string{}
	fps                                       = []**os.File{}
	waitGroup                        sync.WaitGroup
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
			if nil != err || nil == *fps[id] {
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
	fmt.Println("Finish")
}

func ReadFile(path string, ch_sql chan string) {
	defer close(ch_sql)
	// waitGroup.Add(1)
	// defer waitGroup.Done()
	fileHanle, err := os.OpenFile(path, os.O_RDONLY, 0666)
	if err != nil {
		fmt.Printf("open file %s failed\n", path)
		return
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
		ch_sql <- sql_str
	}
	fmt.Printf("Finish read file. Total sql count %04d ignore_count %04d\n", sql_count, ignore_count)

}

func record_one_sql(s <-chan SqlParseResult) {
	// waitGroup.Add(1)
	// defer waitGroup.Done()
	for r := range s {
		if len(r.sql) < 2 {
			continue
		}
		// TODO: record into database

		if nil != r.err {
			error_count++
			error_sql_fp.WriteString(r.sql + "\n")
			error_sql_reason_fp.WriteString(r.err.Error() + "\n")
			error_sql_normal_fp.WriteString(r.normalized + "\n")
			error_sql_digest_fp.WriteString(r.digest + "\n")
		} else {
			success_count++
			success_sql_fp.WriteString(r.sql + "\n")
			success_sql_normal_fp.WriteString(r.normalized + "\n")
		}
	}
	fmt.Printf("Finish record parse sql result: success_count %04d error_count %04d\n", success_count, error_count)
}

type SqlParseResult struct {
	sql        string
	normalized string
	digest     string
	err        error
}

func parse_one_sql(ch_sql <-chan string, s chan<- SqlParseResult) {
	// waitGroup.Add(1)
	// defer waitGroup.Done()
	p := parser.New()
	p.EnableWindowFunc(true)
	for sql := range ch_sql {
		// p.SetSQLMode()
		// p.SetParserConfig()
		// p.SetStrictDoubleTypeCheck()
		// p.ParseSQL()
		stms, _, err := p.Parse(sql, "", "")
		var stm ast.StmtNode = stms[0]
		fmt.Println("sql is ", stm.Text())
		normalized, digest := parser.NormalizeDigest(sql)
		r := SqlParseResult{sql, normalized, digest.String(), err}
		s <- r
	}
	atomic.AddInt32(&finish_pasrse_sql_rountine_count, 1)
	// if all gorountine finish ,then finish
	if atomic.LoadInt32(&finish_pasrse_sql_rountine_count) == int32(*parse_sql_rountine_number) {
		fmt.Printf("This Rountine Finish parse sql\n")
		close(s)
	}
}

func main() {
	// TODO: USE FUNC ARGUMENT. ReadFile2(,parse_one_sql)
	var args = os.Args
	parse_sql_rountine_number = flag.Int("r", 10, "parse sql rountine number")
	input_filename_p := flag.String("i", "input.sql", "input sql file name")
	error_filename_p := flag.String("e", "error.sql", "error sql file name")

	if len(args) < 2 {
		fmt.Println("Usage: program -i input_sql_file [ -e error.sql ] [-r parse_rountine_numver] ")
		return
	}

	flag.Parse()
	fmt.Println("input sql file name is ", *input_filename_p)
	error_sql_filename = *error_filename_p

	if err := init1(); nil != err {
		fmt.Println("init failed")
		return
	}
	fmt.Println("parse_sql_rountine_number ", *parse_sql_rountine_number)
	chSqlRes := make(chan SqlParseResult, *parse_sql_rountine_number)
	chSqlStr := make(chan string, *parse_sql_rountine_number)
	go ReadFile(*input_filename_p, chSqlStr)

	for i := 0; i < *parse_sql_rountine_number; i++ {
		go parse_one_sql(chSqlStr, chSqlRes)
	}
	record_one_sql(chSqlRes)
	// waitGroup.Wait()
	// time.Sleep(5 * time.Second)
	finish()
}
