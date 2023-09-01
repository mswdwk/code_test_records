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
		fmt.Printf("key:" + string(k))
		//fmt.Printf("v=%V"+ *v)
		fmt.Printf("\tRow:" + string(v.Row))
		fmt.Printf("\tFamily:" + string(v.Family))
		fmt.Printf("\tQualifier:" + string(v.Qualifier))
		fmt.Printf("\tvalue:" + string(v.Value))
		fmt.Printf("\tcellType:" + string(*v.CellType))
		fmt.Println("\ttags:" + string(v.Tags))
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
	fmt.Printf("get table %s rowkey %s\n", tablename, rowkey)
	displayCells(getRsp)
}

func TestPutOneRow(tablename string, rowkey string, cf string, field string, value string) {
	values := map[string]map[string][]byte{cf: map[string][]byte{field: []byte(value)}}
	putRequest, err := hrpc.NewPutStr(context.Background(), tablename, rowkey, values)
	resp, err := hbaseClient.Put(putRequest)

	if err != nil {
		fmt.Println("hbase client put row error:" + err.Error())
		return
	}
	fmt.Printf("table %s put rowkey %s [%s:%s] value[%s] , resp partial %t\n",
		tablename, rowkey, cf, field, value, resp.Partial)
}

func TestCheckAndPut(tablename string, rowkey string, cf string, field string, oldvalue string, newvalue string) {

	// oldValueMap := map[string]map[string][]byte{cf: map[string][]byte{field: []byte(oldvalue)}}
	// oldValue, err := json.Marshal(oldValueMap)

	newValueMap := map[string]map[string][]byte{cf: map[string][]byte{field: []byte(newvalue)}}
	newRequest, err := hrpc.NewPutStr(context.Background(), tablename, rowkey, newValueMap)

	ret, err := hbaseClient.CheckAndPut(newRequest, cf, field, []byte(oldvalue))

	if err != nil {
		fmt.Println("hbase client put row error:" + err.Error())
		return
	}
	fmt.Printf("check and put table %s rowkey %s [%s:%s] oldV[%s] newV[%s], ret %t\n",
		tablename, rowkey, cf, field, oldvalue, newvalue, ret)
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
