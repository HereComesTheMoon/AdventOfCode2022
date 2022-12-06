package main

import (
	"fmt"
	"io"
	"os"
)

const FILE = "./../data/6.txt"

func main() {
	data := read()

	in_window := map[byte]struct{}{}

	i := 0
	for j, v := range data {
		if _, ok := in_window[v]; ok {
			for data[i] != v {
				delete(in_window, data[i])
				i++
			}
			i++
		}
		in_window[v] = struct{}{};
		if j - i == 13 {
			fmt.Printf("The first start-of-message marker is at position %d\n", j + 1)
			break
		}
	}
}


func read() []byte {
	f, err := os.Open(FILE)
	defer f.Close()
	if err != nil { panic(err) }
	// f := strings.NewReader("mjqjpqmgbljsphdztnvjfqwrcgsmlb")

	result, err := io.ReadAll(f)
	if err != nil { panic(err) }
	return result
}
