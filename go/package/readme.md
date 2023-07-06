# [官方golang包管理神器，值得一试](https://zhuanlan.zhihu.com/p/142176662)
# https://zhuanlan.zhihu.com/p/272107375
# https://zhuanlan.zhihu.com/p/109828249 第一个例子测试不通过

如果要引用aPackage目录下面的t.go中的A()函数，则需要import "examp/aPackage".
其中examp是package目录下go.mod中定义的整体的包名称。

go.mod文件 可以通过 命令 go mod init "examp" 生成。

