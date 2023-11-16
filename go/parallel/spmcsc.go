package main

import (
	//"flag"
	"fmt"
	"sync"
	"time"
)

/*func createStr(str chan<- string, number int, close_input_flag bool) {
	for i := 0; i < number; i++ {
		str <- fmt.Sprintf("this is str %d", i)
	}

	if close_input_flag {
		close(str)
	}
}*/
func ConsumeStr(x int, input <-chan string, outCh chan<- string,waitGroup *sync.WaitGroup) {
	defer (*waitGroup).Done()
	counter := 0
	for in := range input {
		counter++
		fmt.Printf("runtine %2d input str: %s\n", x, in)
		outCh <- fmt.Sprintf("Consume str [%s]", in)
		// do something
		time.Sleep(1 * time.Second)
	}
	fmt.Printf("Consumer runtine %3d finish run, receive %d times\n", x, counter)
}

func recordResult(resultCh <-chan string) {
	counter:=0
	for result := range resultCh {
		fmt.Printf("\tResult %3d => %s\n",counter , result)
		counter++
	}
	fmt.Println("Finish Record " , counter ," result."  )
}

// one producer multi consumer one consumer
func OPMCOC(number int, close_input_flag bool) {
	var waitGroup sync.WaitGroup
	inputCh := make(chan string, number)
	resultCh := make(chan string, number)
	fmt.Printf("%#v\n", waitGroup)
	for i := 0; i < number; i++ {
		waitGroup.Add(1)
		go ConsumeStr(i, inputCh,resultCh,&waitGroup)
	}

	fmt.Printf("%#v\n", waitGroup)

	go createStr(inputCh, 2*number, close_input_flag)
	go recordResult(resultCh)
	// wait consumter finish
	waitGroup.Wait()
	fmt.Println("\nClose resultCh...")
	close(resultCh)
	fmt.Println("\nExiting...")
}
