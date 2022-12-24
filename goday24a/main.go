package main

import (
	"bufio"
	"fmt"
	"os"
)

const (
	WALL  byte = 1 << iota
	N     byte = 1 << iota
	E     byte = 1 << iota
	S     byte = 1 << iota
	W     byte = 1 << iota
	START byte = 1 << iota
	GOAL  byte = 1 << iota
)

type Queue [][3]uint

func (q *Queue) pop() [3]uint {
	a := (*q)[0]
	(*q) = (*q)[1:]
	return a
}

func (q *Queue) push(el [3]uint) {
	(*q) = append(*q, el)
}

func (q Queue) is_empty() bool {
	return len(q) == 0
}

type Grid struct {
	cycle uint
	g [][][]byte
	temp [][]byte
}

func gcd(x, y uint) uint {
	if x < y {
		x, y = y, x
	}
	if y == 0 {
		return x
	}
	return gcd(y, x - y * (x / y))
}

func read() (Grid, error) {
	f, err := os.Open("./../data/24.txt")
	defer f.Close()
	if err != nil {
		return Grid{}, err
	}
	s := bufio.NewScanner(f)
	s.Split(bufio.ScanLines)

	grid := Grid{}
	grid.g = make([][][]byte, 1)
	for s.Scan() {
		grid.g[0] = append(grid.g[0], make([]byte, len(s.Text())))
		grid.temp = append(grid.temp, make([]byte, len(s.Text())))
		for i, v := range s.Text() {
			switch v {
			case '#': grid.g[0][len(grid.g[0])-1][i] = WALL
			case '^': grid.g[0][len(grid.g[0])-1][i] = N
			case '>': grid.g[0][len(grid.g[0])-1][i] = E
			case 'v': grid.g[0][len(grid.g[0])-1][i] = S
			case '<': grid.g[0][len(grid.g[0])-1][i] = W
			case '.':
			default: fmt.Println("This should not happen.")
			}
		}
	}
	grid.g[0][0][1] = START;
	grid.g[0][len(grid.g[0]) - 1][len(grid.g[0][0]) - 2] = GOAL;
	a := uint(len(grid.g[0]) - 2)
	b := uint(len(grid.g[0][0]) - 2)
	grid.cycle = a * b / gcd(a, b)
	fmt.Printf("Height: %v, Width: %v. Cycle: %v", len(grid.g[0]), len(grid.g[0][0]), grid.cycle)

	return grid, nil
}

func (g Grid) Print(i uint) {
	for _, row := range g.g[i] {
		fmt.Println()
		for _, val := range row {
			switch val {
			case WALL: fmt.Print("#")
			case N: fmt.Print("^")
			case E: fmt.Print(">")
			case S: fmt.Print("v")
			case W: fmt.Print("<")
			case 0: fmt.Print(".")
			case START: fmt.Print("S")
			case GOAL: fmt.Print("G")
			default: fmt.Print("O")
			}
		}
	}
	fmt.Println()
}

func (g *Grid) step() bool {
	n := uint(len(g.g))
	if g.cycle <= n {
		return false
	}

	g.g = append(g.g, make([][]byte, len(g.g[0])))

	// fmt.Printf("%v\n", g.g[0])
	// fmt.Printf("%v\n", g.g[1])
	a := len(g.g[n-1]) - 2
	b := len(g.g[n-1][0]) - 2
	for y, row := range g.g[n-1] {
		g.g[n][y] = make([]byte, len(g.g[n-1][0]))
		for x, val := range row {
			if val == WALL || val == START || val == GOAL {
				g.g[n][y][x] = val
				continue;
			}
			g.g[n][y][x] = 0
			if g.g[n-1][y-1][x] & S == S { g.g[n][y][x] |= S }
			if g.g[n-1][y+1][x] & N == N { g.g[n][y][x] |= N }
			if g.g[n-1][y][x-1] & E == E { g.g[n][y][x] |= E }
			if g.g[n-1][y][x+1] & W == W { g.g[n][y][x] |= W }
			if y == 1 && g.g[n-1][a][x] & S == S { g.g[n][y][x] |= S }
			if y == a && g.g[n-1][1][x] & N == N { g.g[n][y][x] |= N }
			if x == 1 && g.g[n-1][y][b] & E == E { g.g[n][y][x] |= E }
			if x == b && g.g[n-1][y][1] & W == W { g.g[n][y][x] |= W }
		}
	}

	return true
}

func (g Grid) valid(v [3]uint) bool {
	return g.g[v[2] % g.cycle][v[1]][v[0]] == 0
}

func (g Grid) search() uint {
	// Contains indices, x, y, time
	q := Queue{[3]uint{1, 1, 1}}
	for !q.is_empty() {
		el := q.pop()
		x, y, time := el[0], el[1], el[2]
		// fmt.Printf("x: %v, y: %v\n", x, y)
		if g.g[0][y+1][x] == GOAL {
			return time + 1
		}
		next := [5][3]uint{
			{x  , y  , time+1},
			{x+1, y  , time+1},
			{x-1, y  , time+1},
			{x  , y+1, time+1},
			{x  , y-1, time+1},
		}
		for _, v := range next {
			if g.valid(v) {
				q.push(v)
			}
		}
	}

	return 0
}

func main() {
	fmt.Printf("Hello world!\n")
	g, err := read()
	if err != nil { panic(1) }

	for g.step() {}

	res := g.search()
	fmt.Printf("The result is: %v\n", res)
}
