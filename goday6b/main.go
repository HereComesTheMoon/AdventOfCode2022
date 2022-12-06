package main

import (
	"fmt"
	"io"
	"os"
	"runtime"
)

func main() {
	f, err := os.Open("./../bigboy/6.txt")
	defer f.Close()
	if err != nil {
		panic(err)
	}
	solve(f, runtime.NumCPU()+1)
}

func solve(input io.Reader, threads int) int {
	data, err := io.ReadAll(input)
	if err != nil {
		panic(err)
	}
	fmt.Println("File read.")

	chunk_size := len(data)/threads + 1
	chans := make([]chan int, 0, threads)
	for i := 0; i < threads; i++ {
		ch := make(chan int)
		chans = append(chans, ch)
		go compute(data[i*chunk_size:(i+1)*chunk_size+14], ch)
	}

	for i := 0; i < threads; i++ {
		fmt.Printf("Waiting for %d...\n", i)
		result := <-chans[i]
		fmt.Printf("Received %d\n", result)
		if result == -1 {
			continue
		}
		fmt.Printf("The first start-of-message marker is at position %d\n", i*chunk_size+result)
		return i*chunk_size + result
	}

	return -1
}

func compute(chunk []byte, ch chan int) {
	in_window := map[byte]struct{}{}

	i := 0
	for j, v := range chunk {
		if _, ok := in_window[v]; ok {
			for chunk[i] != v {
				delete(in_window, chunk[i])
				i++
			}
			i++
		}
		in_window[v] = struct{}{}
		if j-i == 13 {
			ch <- j + 1
			break
		}
	}
	ch <- -1
}
