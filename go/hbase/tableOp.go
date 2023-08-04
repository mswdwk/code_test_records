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

func init() {
	table = fmt.Sprintf("gohbase_test_%d", time.Now().UnixNano())
}

func TestTableOpMain(delete_table bool) {
	if host == nil {
		panic("Host is not set!")
	}

	log.SetLevel(log.DebugLevel)

	ac := gohbase.NewAdminClient(*host)

	var err error
	for {
		err = CreateTable(ac, table, []string{"cf", "cf1", "cf2"})
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
		err = DeleteTable(ac, table)
		if err != nil {
			panic(err)
		}
	}
	log.Debug("table op finish")
	fmt.Println("table op finish")
	// os.Exit(0)
}

// CreateTable creates the given table with the given families
func CreateTable(client gohbase.AdminClient, table string, cFamilies []string) error {
	// If the table exists, delete it
	DeleteTable(client, table)
	// Don't check the error, since one will be returned if the table doesn't
	// exist

	cf := make(map[string]map[string]string, len(cFamilies))
	for _, f := range cFamilies {
		cf[f] = nil
	}

	// pre-split table for reverse scan test of region changes
	keySplits := [][]byte{[]byte("REVTEST-100"), []byte("REVTEST-200"), []byte("REVTEST-300")}
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
