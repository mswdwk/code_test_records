package main

import (
	"fmt"
	"io"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)

func AuthMiddleWare() gin.HandlerFunc {
	return func(c *gin.Context) {
		// 获取客户端cookie并校验
		if cookie, err := c.Cookie("abc"); err == nil {
			if cookie == "123" {
				c.Next()
				return
			}
		}
		// 返回错误
		c.JSON(http.StatusUnauthorized, gin.H{"error": "err"})
		// 若验证不通过，不再调用后续的函数处理
		c.Abort()
		return
	}
}

func main() {

	// Logging to a file.
	f, _ := os.Create("gin.log")
	gin.DefaultWriter = io.MultiWriter(f)

	// 如果需要同时将日志写入文件和控制台，请使用以下代码。
	// gin.DefaultWriter = io.MultiWriter(f, os.Stdout)

	// 1.创建路由
	r := gin.Default()
	r.GET("/login", func(c *gin.Context) {
		// 设置cookie
		c.SetCookie("abc", "123", 60, "/",
			"localhost", false, true)
		// 返回信息
		c.String(200, "Login success!")
	})
	r.GET("/home", AuthMiddleWare(), func(c *gin.Context) {
		c.JSON(200, gin.H{"data": "home"})
	})
	err := r.Run(":8000")
	if nil != err {
		fmt.Println("gin run err: ", err)
	}
}
