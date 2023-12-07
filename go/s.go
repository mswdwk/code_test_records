package main

import "fmt"

func main() {
	asString := "11"
	switch asString {
	case "1":
		fmt.Println("One")
	case "0":
		fmt.Println("Zero")
	default:
		fmt.Println("Do not care!")
	}

	fmt.Println("exit !")
}
