package main

import (
	"testing"
)


func Test_gcd(t *testing.T) {
	if gcd(6, 4) != 2 { t.Fail() }
	if gcd(4, 6) != 2 { t.Fail() }
	if gcd(18, 4) != 2 { t.Fail() }
	if gcd(1, 4) != 1 { t.Fail() }
	if gcd(99, 99) != 99 { t.Fail() }
	if gcd(19, 23) != 1 { t.Fail() }
	if gcd(64, 4) != 4 { t.Fail() }
	if gcd(64*25*81, 33) != 3 { t.Fail() }
}

func Test_queue(t *testing.T) {
	q := Queue{}
	q.Push([3]uint{0,0,0})
	q.Push([3]uint{1,1,1})
	q.Push([3]uint{2,2,2})
	if q.Pop() != [3]uint{0,0,0} {
		t.Fail()
	}
	if q.Pop() != [3]uint{1,1,1} {
		t.Fail()
	}
	if q.Pop() != [3]uint{2,2,2} {
		t.Fail()
	}
	q.Push([3]uint{0,0,0})
	if q.Pop() != [3]uint{0,0,0} {
		t.Fail()
	}
}

func Test_inp_a(t *testing.T) {
	g, err := read("./../data/24t.txt")
	if err != nil { panic(1) }

	for g.step() {}

	var x1 uint = uint(len(g.g[0][0]) - 2)
	var y1 uint = uint(len(g.g[0]) - 1)
	res := g.search(1, 0, 0, x1, y1)
	if res != 18 {
		t.Fail()
	}
}

func Test_inp_b(t *testing.T) {
	g, err := read("./../data/24t.txt")
	if err != nil { panic(1) }

	for g.step() {}

	var x1 uint = uint(len(g.g[0][0]) - 2)
	var y1 uint = uint(len(g.g[0]) - 1)
	res1 := g.search(1, 0, 0, x1, y1)
	res2 := g.search(x1, y1, res1, 1, 0) - res1
	res3 := g.search(1, 0, res1 + res2, x1, y1) - res2 - res1
 
	if res1 + res2 + res3 != 54 {
		t.Fail()
	}
}
