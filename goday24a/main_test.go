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
	q.push([3]uint{0,0,0})
	q.push([3]uint{1,1,1})
	q.push([3]uint{2,2,2})
	if q.pop() != [3]uint{0,0,0} {
		t.Fail()
	}
	if q.pop() != [3]uint{1,1,1} {
		t.Fail()
	}
	if q.pop() != [3]uint{2,2,2} {
		t.Fail()
	}
	q.push([3]uint{0,0,0})
	if q.pop() != [3]uint{0,0,0} {
		t.Fail()
	}
}
