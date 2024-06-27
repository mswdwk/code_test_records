// main.go
// https://zhuanlan.zhihu.com/p/616926076
package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/gin-gonic/gin" // 引入我们要的用的gin框架
)

// 创建日志文件并返回一个*log.Logger实例
func newLogger() *log.Logger {
	file, err := os.OpenFile("gin.log", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0666)
	if err != nil {
		log.Fatal(err)
	}
	return log.New(file, "", log.LstdFlags)
}

// 定义中间
func MiddleWare() gin.HandlerFunc {
	return func(c *gin.Context) {
		t := time.Now()
		fmt.Println("中间件开始执行了")
		// 设置变量到Context的key中，可以通过Get()取
		c.Set("request", "中间件")

		// c.Next() 是让调该Handler 执行下一个Handler.
		// 执行函数
		c.Next()

		status := c.Writer.Status()
		fmt.Println("中间件执行完毕 ", status)
		t2 := time.Since(t)
		fmt.Println("time:", t2)
	}
}

// 此步骤添加
func main() {
	gin.SetMode(gin.ReleaseMode)
	router := gin.Default()

	// 创建日志实例
	logger := newLogger()
	router.Use(MiddleWare())

	// 自定义中间件，用于记录请求日志
	router.Use(func(c *gin.Context) {
		// 在这里记录请求日志
		logger.Println(c.Request.Method, c.Request.URL, c.Request.RemoteAddr)
	})

	router.GET("/albums", getAlbums)        // 路由写在这里
	router.GET("/albums/:id", getAlbumByID) // 路由写在这里
	router.POST("/albums", postAlbums)
	router.MaxMultipartMemory = 8 << 20
	router.POST("/upload", func(c *gin.Context) {
		// file, err := c.FormFile("file")
		_, headers, err := c.Request.FormFile("file")
		if err != nil {
			c.String(500, "上传图片出错")
		}
		// c.JSON(200, gin.H{"message": file.Header.Context})
		//headers.Size 获取文件大小
		if headers.Size > 1024*1024*2 {
			fmt.Println("文件太大了")
			c.String(http.StatusForbidden, headers.Filename)
			return
		}
		//headers.Header.Get("Content-Type")获取上传文件的类型
		if headers.Header.Get("Content-Type") != "image/png" {
			fmt.Println("只允许上传png图片")
			c.String(http.StatusBadRequest, headers.Filename)
			return
		}
		c.SaveUploadedFile(headers, "./store/"+headers.Filename)
		c.String(http.StatusOK, headers.Filename)
	})

	v1 := router.Group("/v1")
	// {} 是书写规范
	{
		v1.GET("/login", login)
		v1.GET("submit", submit)
	}
	v2 := router.Group("/v2")
	{
		v2.POST("/login", login)
		v2.POST("/submit", submit)
	}
	router.GET("/health", func(c *gin.Context) {
		// 取值
		req, _ := c.Get("request")
		// fmt.Println("request:", req)
		// 页面接收
		// // 页面接收
		c.JSON(200, gin.H{"request": req})
		//c.String(200, "ok")
	})
	router.Run("0.0.0.0:8081")
}

func login(c *gin.Context) {
	name := c.DefaultQuery("name", "jack")
	c.String(200, fmt.Sprintf("hello %s\n", name))
}

func submit(c *gin.Context) {
	name := c.DefaultQuery("name", "lily")
	c.String(200, fmt.Sprintf("hello %s\n", name))
}

// 控制器函数 gin.Context 是框架的上下问
func getAlbums(c *gin.Context) {
	// c.IndentedJSON是返回缩减的json
	c.IndentedJSON(http.StatusOK, albums)
}

// 添加条目到albums
func postAlbums(c *gin.Context) {
	// 定义一个变量 为结构体类型album
	var newAlbum album

	// 调用 BindJSON 绑定接受到的json到 newAlbum变量
	if err := c.BindJSON(&newAlbum); err != nil {
		return
	}

	// 将newAlbum 加入到albums中,这里是加到内存中的
	albums = append(albums, newAlbum)
	c.IndentedJSON(http.StatusCreated, newAlbum)
}

// main.go
func getAlbumByID(c *gin.Context) {
	id := c.Param("id") // 路径参数获取值

	// 循环找到id
	for _, a := range albums {
		if a.ID == id {
			c.IndentedJSON(http.StatusOK, a)
			return
		}
	}
	c.IndentedJSON(http.StatusNotFound, gin.H{"message": "album not found"})
}

// 结构体 ID（字段） String(字段类型)
type album struct {
	ID     string  `json:"id"`
	Title  string  `json:"title"`
	Artist string  `json:"artist"`
	Price  float64 `json:"price"`
}

// 用新的结构体类型 定义一个唱片集合
var albums = []album{
	{ID: "1", Title: "Blue Train", Artist: "John Coltrane", Price: 56.99},
	{ID: "2", Title: "Jeru", Artist: "Gerry Mulligan", Price: 17.99},
	{ID: "3", Title: "Sarah Vaughan and Clifford Brown", Artist: "Sarah Vaughan", Price: 39.99},
}
