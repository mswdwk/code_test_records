package main

import (
	//"flag"
	"fmt"
	"sync"
	"time"
)

func createStr(str chan<- string, number int, close_input_flag bool) {
	for i := 0; i < number; i++ {
		str <- fmt.Sprintf("this is str %03d", i)
	}

	if close_input_flag {
		fmt.Println("\nClose Create String\n")
		close(str)
	}
}

func OneProdMultiCons(number int, close_input_flag bool) {
	var waitGroup sync.WaitGroup
	input := make(chan string, number)
	fmt.Printf("%#v\n", waitGroup)
	for i := 0; i < number; i++ {
		waitGroup.Add(1)
		go func(x int, input <-chan string) {
			defer waitGroup.Done()
			numberer := 0
			for in := range input {
				numberer++
				fmt.Printf("%d ,input str: %s\n", x, in)
				// do something
				time.Sleep(1 * time.Second)
			}
			fmt.Printf("%d ,finish run, receive %d times\n", x, numberer)
		}(i, input)
	}

	fmt.Printf("%#v\n", waitGroup)

	go createStr(input, 2*number, close_input_flag)
	waitGroup.Wait()
	fmt.Println("\nExiting...")
}
