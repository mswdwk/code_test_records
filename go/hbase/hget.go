package main

import (
        "context"
        "fmt"

        "github.com/tsuna/gohbase"
        "github.com/tsuna/gohbase/hrpc"
)

func main() {
        hbase_client := gohbase.NewClient("192.168.79.134") // hbase服务器地址,只有ip，没有port。
        // hbase_client := gohbase.NewClient("127.0.0.1")
        getRequest, err := hrpc.NewGetStr(context.Background(), "student", "00000000000000000009512345")
        getRsp, err := hbase_client.Get(getRequest) // Get()方法返回查询结果。通过客户端真正读取数据
        if err != nil {
                fmt.Println("hbase get client error:" + err.Error())
                return
        }
        // create 'member','member_id','address','info'
        type mystruct struct {
                Use    string               `json:"user_id" `
                Movies map[string][]float64 `json:"movies" ` // 用户看的多部电影 "电影id":[打分int,喜好程度float]
        }
        for k, v := range getRsp.Cells { // v结构体中的Value保存了真正的数据
                // value := v.Value
                fmt.Println("key:"+string(k))
                //fmt.Printf("v=%V"+ *v)
                fmt.Println("Row:" + string(v.Row))
                fmt.Println("Family:" + string(v.Family))
                fmt.Println("Qualifier:" + string(v.Qualifier))
                fmt.Println("value:" + string(v.Value))
                //      var myuser mystruct
                //      err := json.Unmarshal(value, &myuser) // value为 []unit8类型的字节数组，所以可以直接放到json.Unmarshal
                //      if err != nil {
                //              fmt.Println(err.Error())
                //      }
                //      fmt.Println(myuser)
        }
}
