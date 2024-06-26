// main.go
// https://zhuanlan.zhihu.com/p/616926076
package main 

import(
    "os"
    "log"
    "net/http"
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

// 此步骤添加
func main() {
    router := gin.Default()

    // 创建日志实例
    logger := newLogger()
    
    // 自定义中间件，用于记录请求日志
    router.Use(func(c *gin.Context) {
        // 在这里记录请求日志
        logger.Println(c.Request.Method, c.Request.URL, c.Request.RemoteAddr)
    })

    router.GET("/albums", getAlbums) // 路由写在这里
    router.GET("/albums/:id", getAlbumByID) // 路由写在这里
  	router.POST("/albums", postAlbums)

    router.Run("0.0.0.0:8081")
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
