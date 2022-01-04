package main

import (
	"fmt"
	"log"

	"example.com/greetings"
)

func fibonacci(n int, c chan int) {
	x, y := 0, 1
	for i := 0; i < n; i++ {
		c <- x
		x, y = y, x+y
	}
	close(c)
}

func main() {
	dirname := "C:\\Users\\lenovo\\Desktop\\"
	greetings.ListFiles(dirname, 0)
	log.SetPrefix("greetings:")
	log.SetFlags(0)
	msg, err := greetings.Hello("")
	if err != nil {
		fmt.Println("error")
		log.Fatal(err)
	}

	fmt.Println(msg)
	c := make(chan int, 10)
	go fibonacci(cap(c), c)
	// range 函数遍历每个从通道接收到的数据，因为 c 在发送完 10 个
	// 数据之后就关闭了通道，所以这里我们 range 函数在接收到 10 个数据
	// 之后就结束了。如果上面的 c 通道不关闭，那么 range 函数就不
	// 会结束，从而在接收第 11 个数据的时候就阻塞了。
	var id = 0
	for i := range c {
		fmt.Println(id, "", i)
		id += 1
	}
	fmt.Println("finish")

}
