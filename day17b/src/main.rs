use std::fmt::Debug;
use std::collections::VecDeque;

const STEPS_UNTIL_CYCLE: u128 = 97;
const CYCLE_LENGTH: u128 = 1715;
const INPUT_FILE: &str = include_str!("./../../data/17.txt");

// const STEPS_UNTIL_CYCLE: u128 = 15;
// const CYCLE_LENGTH: u128 = 35;
// const INPUT_FILE: &str = include_str!("./../../data/17t.txt");

struct Tile {
    tiles: Vec<(usize, usize)>,
    width: usize,
    height: usize,
}

struct Tetris {
    screen: Vec<[bool; 7]>,
    peaks: Vec<isize>,
    input: VecDeque<bool>,
    next_tile: u8,
    number_rocks: u128,
    tiles: [Tile; 5],
}

impl Tetris {
    fn new(input: Vec<bool>) -> Self {
        Tetris {
            screen: vec![],
            peaks: vec![-1],
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
        let mut y = (3 + self.peaks[self.peaks.len() - 1] + tile.height as isize) as usize;
        self.number_rocks += 1;
        while self.screen.len() <= y {
            self.screen.push([false; 7]);
        }

        loop {
            if self.input.pop_back().unwrap() {
                self.input.push_front(true);
                if self.check_free(x as isize + 1, y, tile) {
                    x = (x + 1).min(7 - tile.width);
                }
            } else {
                self.input.push_front(false);
                if self.check_free(x as isize - 1, y, tile) {
                    x = x.saturating_sub(1);
                }
            }
            if y == tile.height - 1 { break; }
            if self.check_free(x as isize, y - 1, tile) {
                y -= 1;
            } else {
                break;
            }
        }
        for (a, b) in tile.tiles.iter() {
            self.screen[y - b][x + a] = true;
        }
        self.peaks.push(self.peaks[self.peaks.len() - 1].max(y as isize));
        self.next_tile = (self.next_tile + 1) % self.tiles.len() as u8;
    }

    fn check_free(&self, x: isize, y: usize, tile: &Tile) -> bool {
        if x < 0 || self.screen[0].len() <= x as usize { return false; }
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

    fn find_cycle(&mut self, start_pos: u128, min_cycle_size: usize) -> Vec<u8> {
        for _ in 0..start_pos {
            self.fall();
        }
        let mut steps = vec![];
        for _ in 0..min_cycle_size {
            self.fall();
            steps.push(self.peaks.last().unwrap() - self.peaks[self.peaks.len() - 2]);
        }
        let (a, b): (usize, Vec<u8>) = loop {
            if steps[..steps.len() / 2] == steps[steps.len() / 2..] {
                // println!("FOUND! {:?}", &steps[..steps.len() / 2]);
                // println!("LENGTH: {}", steps.len() / 2);
                break (steps.len() / 2, steps[..steps.len() / 2].iter().map(|&x| x as u8).collect());
            }
            self.fall();
            steps.push(self.peaks.last().unwrap() - self.peaks[self.peaks.len() - 2]);
            self.fall();
            steps.push(self.peaks.last().unwrap() - self.peaks[self.peaks.len() - 2]);
        };

        b
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
    INPUT_FILE
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

fn solution(tetris: &Tetris, sum: u128, s: u128) -> u128 {
    tetris.peaks[(STEPS_UNTIL_CYCLE + (s - STEPS_UNTIL_CYCLE) % CYCLE_LENGTH) as usize] as u128 + ((s - STEPS_UNTIL_CYCLE) / CYCLE_LENGTH) * sum + 1
}


fn main() {
    let mut tetris = Tetris::new(read());
    let cycle: Vec<u128> = tetris.find_cycle(STEPS_UNTIL_CYCLE, 2).into_iter().map(|x| x as u128).collect();

    for _ in 0..10000 {
        tetris.fall();
    }

    // println!("{:?}", tetris.peaks);
    // println!("{}", cycle.iter().sum::<usize>());
    // peaks[STEPS_UNTIL_CYCLE + (i+1) * CYCLE_LENGTH] -  peaks[STEPS_UNTIL_CYCLE + i * CYCLE_LENGTH] = cycle.sum()
    // a(i+1) - a(i) = sum
    // a(i+1) = sum + a(i) = 2*sum + a(i-1) = N*sum + a(i-(N-1))
    // a(i) = N*sum + a(i - N)
    // SEARCHED_VAL = STEPS_UNTIL_CYCLE + REST +  i * CYCLE_LENGTH
    // i = (SEARCHED_VAL - STEPS_UNTIL_CYCLE - REST) / CYCLE_LENGTH
    // REST = (SEARCHED_VAL - STEPS_UNTIL_CYCLE) % CYCLE_LENGTH

    let sum = cycle.iter().sum::<u128>();

    for x in STEPS_UNTIL_CYCLE..10000 {
        let result = solution(&tetris, sum, x);
        let wanted = 1 + tetris.peaks[x as usize] as u128;
        if result != wanted {
            println!("{}: {} != {}. Diff: {}", x, result, wanted, (result as i128) - (wanted as i128));
        }
    }

    let s: u128 = 1000000000000;

    let result = solution(&tetris, sum, s);

    println!("{}", result);
}
