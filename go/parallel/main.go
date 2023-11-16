package main

import (
	"flag"
	"fmt"
	"sync"
)

func wait_all_goroutines(number int) {
	var waitGroup sync.WaitGroup
	fmt.Printf("%#v\n", waitGroup)
	for i := 0; i < number ; i++ {
		waitGroup.Add(1)
		go func(x int) {
			defer waitGroup.Done()
			fmt.Printf("%d ", x)
		}(i)
	}

	fmt.Printf("%#v\n", waitGroup)
	waitGroup.Wait()
	fmt.Println("\nExiting...")
}

func main(){
	n := flag.Int("n", 20, "Number of goroutines")
	flag.Parse()
	number := *n
	fmt.Printf("Going to create %d goroutines.\n", number)
	wait_all_goroutines(number)
	// this works well
	OneProdMultiCons(number,true)
	// this will panic, because  sync.Add() 的次数大于执行 sync.Done()
	//OneProdMultiCons(number,false)
}
