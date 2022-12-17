use std::fmt::Debug;
use std::collections::VecDeque;

struct Tile {
    tiles: Vec<(usize, usize)>,
    width: usize,
    height: usize,
}

struct Tetris {
    screen: Vec<[bool; 7]>,
    peak: isize,
    input: VecDeque<bool>,
    next_tile: u8,
    number_rocks: u32,
    tiles: [Tile; 5],
}

impl Tetris {
    fn new(input: Vec<bool>) -> Self {
        Tetris {
            screen: vec![],
            peak: -1,
            input: input.into(),
            next_tile: 0,
            number_rocks: 0,
            tiles: [
                Tile { tiles: vec![(0, 0), (1, 0), (2, 0), (3, 0)], width: 4, height: 1, },
                Tile { tiles: vec![(0, 1), (1, 0), (1, 1), (1, 2), (2, 1)], width: 3, height: 3, },
                Tile { tiles: vec![(2, 0), (2, 1), (0, 2), (1, 2), (2, 2)], width: 3, height: 3, },
                Tile { tiles: vec![(0, 0), (0, 1), (0, 2), (0, 3)], width: 1, height: 4, },
                Tile { tiles: vec![(0, 0), (0, 1), (1, 0), (1, 1)], width: 2, height: 2, },
            ],
        }
    }

    fn fall(&mut self) {
        let tile = &self.tiles[self.next_tile as usize];
        let mut x = 2;
        let mut y = (3 + self.peak + tile.height as isize) as usize;
        self.number_rocks += 1;
        while self.screen.len() <= y {
            self.screen.push([false; 7]);
        }
        // {
        //     for (a, b) in tile.tiles.iter() {
        //         println!("x {} y {} a {} b {}", x, y, a, b);
        //         self.screen[y - b][x + a] = true;
        //     }
        //     println!("{:?}", self);
        //     for (a, b) in tile.tiles.iter() {
        //         self.screen[y - b][x + a] = false;
        //     }
        // }

        loop {
            if self.input.pop_back().unwrap() {
                self.input.push_front(true);
                // println!("Rock pushed right >.");
                if self.check_free(x as isize + 1, y, tile) {
                    x = (x + 1).min(7 - tile.width);
                }
            } else {
                // println!("Rock pushed left  <.");
                self.input.push_front(false);
                // x = (x - 1).max(0);
                if self.check_free(x as isize - 1, y, tile) {
                    x = x.saturating_sub(1);
                }
            }
            if y == tile.height - 1 { break; }
            if self.check_free(x as isize, y - 1, tile) {
                y -= 1;
            } else {
                // println!("nooo");
                break;
            }
        }
        for (a, b) in tile.tiles.iter() {
            self.screen[y - b][x + a] = true;
        }
        self.peak = self.peak.max(y as isize);
        self.next_tile = (self.next_tile + 1) % self.tiles.len() as u8;
    }

    fn check_free(&self, x: isize, y: usize, tile: &Tile) -> bool {
        if x < 0 || self.screen[0].len() <= x as usize { return false; }
        // if y <= tile.height - 0 { 
            // println!("jjj");
            // return false; }
        for (a, b) in tile.tiles.iter() {
            if self.screen.len() <= y - b || self.screen[0].len() <= x as usize + a {
                continue;
            }
            if self.screen[y - b][x as usize + a] {
                return false;
            }
        }
        return true;
    }
}

impl Debug for Tetris {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut s = String::new();
        for row in self.screen.iter().rev() {
            s.extend(row.iter().map(|&x| if x { '#' } else { '.' }));
            s.push_str("\n");
        }
        write!(f, "{}", s)
    }
}

fn read() -> Vec<bool> {
    include_str!("./../../data/17.txt")
        .trim_end()
        .as_bytes()
        .into_iter()
        .map(|c| match c {
            b'>' => true,
            b'<' => false,
            _ => unreachable!(),
        })
        .rev()
        .collect()
}

fn main() {
    println!("Hello, world!");
    let mut tetris = Tetris::new(read());

    println!("{:?}", tetris.input);
    for _ in 0..2022 {
        tetris.fall();
        // println!("{:?}", tetris);
    }

    println!("Height: {}", tetris.peak);
}
