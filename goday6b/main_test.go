package main

import (
	"os"
	"runtime"
	"strings"
	"testing"
)

func TestExample(t *testing.T) {
	f := strings.NewReader("mjqjpqmgbljsphdztnvjfqwrcgsmlb")
	res := solve(f, 1)
	wanted := 19
	if res != wanted {
		t.Fatalf("Wanted: %d, Got: %d", wanted, res)
	}
}

func TestExampleMulti(t *testing.T) {
	f := strings.NewReader("mjqjpqmgbljsphdztnvjfqwrcgsmlb")
	res := solve(f, runtime.NumCPU()+1)
	wanted := 19
	if res != wanted {
		t.Fatalf("Wanted: %d, Got: %d", wanted, res)
	}
}

func TestGold(t *testing.T) {
	f, err := os.Open("./../data/6.txt")
	if err != nil {
		t.Fatal("Couldn't open file.")
	}
	res := solve(f, 1)
	wanted := 3605
	if res != wanted {
		t.Fatalf("Wanted: %d, Got: %d", wanted, res)
	}
}

func TestGoldMulti(t *testing.T) {
	f, err := os.Open("./../data/6.txt")
	if err != nil {
		t.Fatal("Couldn't open file.")
	}
	res := solve(f, runtime.NumCPU()+1)
	wanted := 3605
	if res != wanted {
		t.Fatalf("Wanted: %d, Got: %d", wanted, res)
	}
}

func BenchmarkBigBoy(b *testing.B) {
	for k := 0; k < b.N; k++ {
		f, err := os.Open("./../bigboy/6.txt")
		if err != nil {
			b.Fatal("Couldn't open file.")
		}
		res := solve(f, 1)
		wanted := 91845017
		if res != wanted {
			b.Fatalf("Wanted: %d, Got: %d", wanted, res)
		}
	}
}

func BenchmarkBigBoyMulti(b *testing.B) {
	for k := 0; k < b.N; k++ {
		f, err := os.Open("./../bigboy/6.txt")
		if err != nil {
			b.Fatal("Couldn't open file.")
		}
		res := solve(f, runtime.NumCPU() + 1)
		wanted := 91845017
		if res != wanted {
			b.Fatalf("Wanted: %d, Got: %d", wanted, res)
		}
	}
}

