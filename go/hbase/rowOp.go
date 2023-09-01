package main

import (
	"context"
	"fmt"
	"time"

	"github.com/tsuna/gohbase"
	"github.com/tsuna/gohbase/hrpc"
)

var hbaseClient gohbase.Client

func InitHbaseClient() {
	// ip:port 是hbase 连接的 etcd/zkServer
	hbaseClient = gohbase.NewClient(*host) // hbase服务器地址,可以是只有ip没有port，也可以是ip:port。
	if nil == hbaseClient {
		panic("init hbase client failed")
	} else {
		fmt.Println("init hbase client ok")
	}
}

func displayCells(result *hrpc.Result) {
	for k, v := range result.Cells { // v结构体中的Value保存了真正的数据
		// value := v.Value
		fmt.Println("key:" + string(k))
		//fmt.Printf("v=%V"+ *v)
		fmt.Println("Row:" + string(v.Row))
		fmt.Println("Family:" + string(v.Family))
		fmt.Println("Qualifier:" + string(v.Qualifier))
		fmt.Println("value:" + string(v.Value))
		fmt.Println("cellType:" + string(*v.CellType))
		fmt.Println("tags:" + string(v.Tags))
		//      var myuser mystruct
		//      err := json.Unmarshal(value, &myuser) // value为 []unit8类型的字节数组，所以可以直接放到json.Unmarshal
		//      if err != nil {
		//              fmt.Println(err.Error())
		//      }
		//      fmt.Println(myuser)
	}
}

func TestGetMain(tablename string, rowkey string) {
	// hrpc.MaxVersions(3)
	getRequest, err := hrpc.NewGetStr(context.Background(), tablename, rowkey, hrpc.MaxVersions(3))
	// getRequest.maxVersions = 3
	getRsp, err := hbaseClient.Get(getRequest) // Get()方法返回查询结果。通过客户端真正读取数据

	if err != nil {
		fmt.Println("hbase get client error:" + err.Error())
		return
	}
	displayCells(getRsp)
}

func TestPutMain(tablename string, rowkey string, cf string, field string, value string) {
	values := map[string]map[string][]byte{cf: map[string][]byte{field: []byte(value)}}
	putRequest, err := hrpc.NewPutStr(context.Background(), tablename, rowkey, values)
	resp, err := hbaseClient.Put(putRequest)

	if err != nil {
		fmt.Println("hbase clientput row error:" + err.Error())
		return
	}

	fmt.Printf("put rowkey[%s] ok %t\n", rowkey, resp.Stale)
}

func TestDeleteMain() {
	// ip:port 是hbase 连接的 etcd/zkServer
	hbaseClient := gohbase.NewClient(*host) // hbase服务器地址,可以是只有ip没有port，也可以是ip:port。

	values := map[string]map[string][]byte{"cf": {"a": []byte(time.Now().String())}}
	// values := map[string]map[string][]byte{"cf": map[string][]byte{"a": []byte("1")}}
	putRequest, err := hrpc.NewDelStr(context.Background(), "student", "row1", values)
	resp, err := hbaseClient.Delete(putRequest)

	if err != nil {
		fmt.Println("hbase clientput row error:" + err.Error())
		return
	}
	displayCells(resp)

	fmt.Println("delete row ok")
}

// create 'member','member_id','address','info'
type mystruct struct {
	Use    string               `json:"user_id" `
	Movies map[string][]float64 `json:"movies" ` // 用户看的多部电影 "电影id":[打分int,喜好程度float]
}
