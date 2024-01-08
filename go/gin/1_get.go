// main.go
package main 

import(
    "net/http"
    "github.com/gin-gonic/gin" // 引入我们要的用的gin框架
)

// 此步骤添加
func main() {
    router := gin.Default()
    router.GET("/albums", getAlbums) // 路由写在这里
  	router.POST("/albums", postAlbums)

    router.Run("localhost:8081")
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
