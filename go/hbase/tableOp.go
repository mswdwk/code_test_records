package main

import (
	"context"
	"fmt"
	"strings"
	"time"

	log "github.com/sirupsen/logrus"
	"github.com/tsuna/gohbase"
	"github.com/tsuna/gohbase/hrpc"
	// "gotest.tools/gotestsum/log"
)

var table string
var HbaseAc gohbase.AdminClient

func init() {
	table = fmt.Sprintf("gohbase_test_%d", time.Now().UnixNano())
}

func InitHbaseAdminClient() {
	if host == nil {
		panic("Host is not set!")
	}
	HbaseAc = gohbase.NewAdminClient(*host)
	if nil == HbaseAc {
		emsg := fmt.Sprintf("hbase Admin client null,host=%s\n", host)
		panic(emsg)
	} else {
		fmt.Println("init hbase admin client ok")
	}
}

func TestCreateTable(tablename string, cfs []string, maxVersion uint32) {
	// []string{"cf", "cf1", "cf2"}
	err := CreateTable(HbaseAc, tablename, cfs, maxVersion)
	count := 0
	for {
		count += 1
		if err != nil &&
			(strings.Contains(err.Error(), "org.apache.hadoop.hbase.PleaseHoldException") ||
				strings.Contains(err.Error(),
					"org.apache.hadoop.hbase.ipc.ServerNotRunningYetException")) {
			time.Sleep(time.Second * 5)
		} else if err != nil {
			fmt.Printf("create table failed: %s,count=%d\n", tablename, count)
			break
			// panic(err)
		} else {
			fmt.Printf("create table ok: %s,count=%d\n", tablename, count)
			break
		}
	}
}

func TestDeleteTable(tablename string) {
	err := DeleteTable(HbaseAc, tablename)
	if nil != err {
		fmt.Printf("delete table failed: %s", tablename)
	} else {
		fmt.Printf("delete table ok: %s", tablename)
	}
}

func TestTableOpMain(delete_table bool) {
	log.SetLevel(log.DebugLevel)

	var err error
	for {
		err = CreateTable(HbaseAc, table, []string{"cf", "cf1", "cf2"}, 1)
		if err != nil &&
			(strings.Contains(err.Error(), "org.apache.hadoop.hbase.PleaseHoldException") ||
				strings.Contains(err.Error(),
					"org.apache.hadoop.hbase.ipc.ServerNotRunningYetException")) {
			time.Sleep(time.Second)
			continue
		} else if err != nil {
			panic(err)
		} else {
			break
		}
	}
	// res := m.Run()
	if delete_table {
		err = DeleteTable(HbaseAc, table)
		if err != nil {
			panic(err)
		}
	}
	log.Debug("table op finish")
	fmt.Println("table op finish")
	// os.Exit(0)
}

// CreateTable creates the given table with the given families
func CreateTable(client gohbase.AdminClient, table string, cFamilies []string, maxVersion uint32) error {
	// If the table exists, delete it
	// DeleteTable(client, table)

	// Don't check the error, since one will be returned if the table doesn't
	// exist

	cf := make(map[string]map[string]string, len(cFamilies))
	for _, f := range cFamilies {
		cf[f] = nil
	}
	// reference : https://www.cnblogs.com/cnblogs-syui/p/12566642.html
	// pre-split table for reverse scan test of region changes
	keySplits := [][]byte{[]byte("REVTEST-100"), []byte("REVTEST-200"), []byte("REVTEST-300")}
	hrpc.MaxVersions(maxVersion)
	ct := hrpc.NewCreateTable(context.Background(), []byte(table), cf, hrpc.SplitKeys(keySplits))
	if err := client.CreateTable(ct); err != nil {
		return err
	}

	return nil
}

// DeleteTable finds the HBase shell via the HBASE_HOME environment variable,
// and disables and drops the given table
func DeleteTable(client gohbase.AdminClient, table string) error {
	dit := hrpc.NewDisableTable(context.Background(), []byte(table))
	err := client.DisableTable(dit)
	if err != nil {
		if !strings.Contains(err.Error(), "TableNotEnabledException") {
			return err
		}
	}

	det := hrpc.NewDeleteTable(context.Background(), []byte(table))
	err = client.DeleteTable(det)
	if err != nil {
		return err
	}
	return nil
}
