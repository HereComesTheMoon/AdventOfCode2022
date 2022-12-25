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
)

type Queue [][3]uint

func (q *Queue) Pop() [3]uint {
	a := (*q)[0]
	(*q) = (*q)[1:]
	return a
}

func (q *Queue) Push(el [3]uint) {
	(*q) = append(*q, el)
}

type Grid struct {
	cycle uint
	g [][][]byte
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

func read(loc string) (Grid, error) {
	f, err := os.Open(loc)
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
		for i, v := range s.Text() {
			switch v {
			case '#': grid.g[0][len(grid.g[0])-1][i] = WALL
			case '^': grid.g[0][len(grid.g[0])-1][i] = N
			case '>': grid.g[0][len(grid.g[0])-1][i] = E
			case 'v': grid.g[0][len(grid.g[0])-1][i] = S
			case '<': grid.g[0][len(grid.g[0])-1][i] = W
			case '.':
			default : fmt.Println("This should not happen.")
			}
		}
	}
	a := uint(len(grid.g[0]) - 2)
	b := uint(len(grid.g[0][0]) - 2)
	grid.cycle = a * b / gcd(a, b)
	fmt.Printf("Height: %v, Width: %v. Cycle: %v\n", len(grid.g[0]), len(grid.g[0][0]), grid.cycle)

	return grid, nil
}

func (g Grid) Print(i uint) {
	for _, row := range g.g[i] {
		fmt.Println()
		for _, val := range row {
			switch val {
			case WALL : fmt.Print("#")
			case N    : fmt.Print("^")
			case E    : fmt.Print(">")
			case S    : fmt.Print("v")
			case W    : fmt.Print("<")
			case 0    : fmt.Print(".")
			default   : fmt.Print("O")
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

	a := len(g.g[n-1]) - 2
	b := len(g.g[n-1][0]) - 2
	for y, row := range g.g[n-1] {
		g.g[n][y] = make([]byte, len(g.g[n-1][0]))
		for x, val := range row {
			if val == WALL || x == 0 || x == b + 1 || y == 0 || y == a + 1{
				g.g[n][y][x] = val
				continue;
			}
			g.g[n][y][x] = 0
			if g.g[n-1][y-1][x]         & S == S { g.g[n][y][x] |= S }
			if g.g[n-1][y+1][x]         & N == N { g.g[n][y][x] |= N }
			if g.g[n-1][y][x-1]         & E == E { g.g[n][y][x] |= E }
			if g.g[n-1][y][x+1]         & W == W { g.g[n][y][x] |= W }
			if y == 1 && g.g[n-1][a][x] & S == S { g.g[n][y][x] |= S }
			if y == a && g.g[n-1][1][x] & N == N { g.g[n][y][x] |= N }
			if x == 1 && g.g[n-1][y][b] & E == E { g.g[n][y][x] |= E }
			if x == b && g.g[n-1][y][1] & W == W { g.g[n][y][x] |= W }
		}
	}

	return true
}

func (g Grid) valid(v [3]uint) bool {
	if uint(len(g.g[0][0])) <= v[0] || uint(len(g.g[0])) <= v[1] { return false; }
	return g.g[v[2] % g.cycle][v[1]][v[0]] == 0
}

func (g Grid) search(x0, y0, t0, x1, y1 uint) uint {
	// x0, y0, t0 start, x1, y1 goal
	q := Queue{[3]uint{x0, y0, t0}} // Contains indices, x, y, time
	visited := make(map[[3]uint]bool)
	for len(q) > 0 {
		el := q.Pop()
		x, y, time := el[0], el[1], el[2]
		next := [5][3]uint{
			{x  , y  , time+1},
			{x+1, y  , time+1},
			{x-1, y  , time+1},
			{x  , y+1, time+1},
			{x  , y-1, time+1},
		}
		for _, v := range next {
			if v[0] == x1 && v[1] == y1 {
				return v[2]
			}
			if !g.valid(v) { continue }
			if visited[[3]uint{v[0], v[1], v[2] % g.cycle}] { continue }
			visited[[3]uint{v[0], v[1], v[2] % g.cycle}] = true
			q.Push(v)
		}
	}
	panic("This should never happen, assuming the goal can be reached at all.")
}

func main() {
	fmt.Printf("Hello world!\n")
	g, err := read("./../data/24.txt")
	if err != nil { panic(1) }

	for g.step() {}

	g.Print(0)

	var x1 uint = uint(len(g.g[0][0]) - 2)
	var y1 uint = uint(len(g.g[0]) - 1)
	res1 := g.search(1, 0, 0, x1, y1)
	res2 := g.search(x1, y1, res1, 1, 0) - res1
	res3 := g.search(1, 0, res1 + res2, x1, y1) - res2 - res1
 
	
	fmt.Printf("The results are: %v + %v + %v = %v\n", res1, res2, res3, res1 + res2 + res3)
}
