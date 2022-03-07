package main

import (
	"fmt"
	"time"

	"rsc.io/quote"
)

func main() {
	fmt.Println(quote.Go())
	fmt.Println("time:", time.Now())
}
