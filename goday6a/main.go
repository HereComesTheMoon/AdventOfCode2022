package main

import (
	"fmt"
	"io"
	"os"
)

const FILE = "./../data/6.txt"

// This solution is, of course, very naive.
// It made me wonder, though. What's the minimum amount of comparisons needed to check that N elements are not equal to one another?
// I would not be surprised if (N-1)! is the best you can do if checking for equality is all you can do.
// If you can order your elements, sorting works, and is definitely better

func main() {
	data := read()

	for k := 3; k < len(data); k++ {
		if data[k] == data[k-1] {
			continue
		}
		if data[k] == data[k-2] {
			continue
		}
		if data[k] == data[k-3] {
			continue
		}
		if data[k-1] == data[k-2] {
			continue
		}
		if data[k-1] == data[k-3] {
			continue
		}
		if data[k-2] == data[k-3] {
			continue
		}
		fmt.Printf("The first start-of-packet marker is at position %d\n", k + 1)
		break
	}

}


func read() []byte {
	f, err := os.Open(FILE)
	defer f.Close()
	if err != nil { panic(err) }
	// f = strings.NewReader("mjqjpqmgbljsphdztnvjfqwrcgsmlb")

	result, err := io.ReadAll(f)
	if err != nil { panic(err) }
	return result
}
