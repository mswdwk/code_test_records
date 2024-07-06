
# error 1 , go语言报错package is not in std go语言的包 转
https://blog.51cto.com/u_16213559/11107687
# error 2, named files must all be in one directory; have gRPC and .

# error 3  , about protobuf
## missing method mustEmbedUnimplemented
```
// 新版protoc-gen-go不支持grpc服务生成，需要通过protoc-gen-go-grpc生成grpc服务接口，但是生成的Server端接口中会出现一个mustEmbedUnimplemented***方法，是为了解决前向兼容问题，如果不解决，就无法传递给RegisterXXXService方法，解决办法有两个：
1. 在grpc server实现结构体中匿名嵌入Unimplemented***Server结构体
  type HelloServer struct {
    proto.UnimplementedHelloServer
  }
2. 使用protoc生成server代码时命令行加上关闭选项protoc --go-grpc_out=require_unimplemented_servers=false

//protoc --go_out=. --go_opt=paths=source_relative proto/helloworld.proto
//protoc --go-grpc_out=. --go-grpc_opt=require_unimplemented_servers=false --go-grpc_opt=paths=source_relative proto/helloworld.proto

```


# reference 
[7-protobuf和gRPC 
](https://www.cnblogs.com/liuqingzheng/p/16271928.html)


https://www.topgoer.com/%E5%BE%AE%E6%9C%8D%E5%8A%A1/GoMicro%E5%85%A5%E9%97%A8.html

[gRPC构建微服务](https://www.topgoer.com/%E5%BE%AE%E6%9C%8D%E5%8A%A1/gRPC%E6%9E%84%E5%BB%BA%E5%BE%AE%E6%9C%8D%E5%8A%A1.html)