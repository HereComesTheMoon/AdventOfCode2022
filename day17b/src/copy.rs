use std::fmt::Debug;
use std::collections::VecDeque;

// const STEPS_UNTIL_CYCLE: usize = 97;
// const CYCLE_LENGTH: usize = 1715;

const STEPS_UNTIL_CYCLE: usize = 15;
const CYCLE_LENGTH: usize = 35;

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
        self.peaks.push(self.peaks[self.peaks.len() - 1].max(y as isize));
        self.next_tile = (self.next_tile + 1) % self.tiles.len() as u8;
        // if 2000 <= self.screen.len() {
            // self.screen = self.screen[1500..].iter().map(|&x| x).collect();
        // }
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

    fn find_cycle(&mut self, start_pos: usize, min_cycle_size: usize) -> Vec<u8> {
        for _ in 0..start_pos { // This is enough
            self.fall();
        }
        let mut steps = vec![];
        // let mut temp = self.peaks.last().unwrap();
        for _ in 0..min_cycle_size {
            self.fall();
            steps.push(self.peaks.last().unwrap() - self.peaks[self.peaks.len() - 2]);
            // temp = self.peak;
        }
        let (a, b): (usize, Vec<u8>) = loop {
            if steps[..steps.len() / 2] == steps[steps.len() / 2..] {
                println!("FOUND! {:?}", &steps[..steps.len() / 2]);
                println!("LENGTH: {}", steps.len() / 2);
                // return steps[..steps.len() / 2].iter().map(|&x| x as u8).collect();
                break (steps.len() / 2, steps[..steps.len() / 2].iter().map(|&x| x as u8).collect());
            }
            self.fall();
            // steps.push(self.peak - temp);
            steps.push(self.peaks.last().unwrap() - self.peaks[self.peaks.len() - 2]);
            // temp = self.peak;
            self.fall();
            steps.push(self.peaks.last().unwrap() - self.peaks[self.peaks.len() - 2]);
            // steps.push(self.peak - temp);
            // temp = self.peak;
        };

        b

        // let mut c: Vec<u8> = vec![];
        // for i in 0..b.len()-1 {
        //     println!("{}: {} {}", i, b[i+1], b[i]);
        //     c.push((b[i+1] - b[i]) as u8);
        // }

        // return c;

        // assert_eq!(self.peaks[start_pos + min_cycle_size + 1] - self.peaks[start_pos + min_cycle_size ], b[0]);
        // assert_eq!(self.peaks[start_pos + min_cycle_size + 2] - self.peaks[start_pos + min_cycle_size + 1], b[1]);
        // for pos in start_pos.. start_pos + 20 {
        //     println!("{} : {} {}", pos, self.peaks[pos] - self.peaks[pos-1], b[pos - start_pos]);
        // }


    }

    // fn find_cycle_start(&self, cycle: Vec<u8>) -> (usize, Vec<u8>) {
        // for 
        // todo!()
    // }
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

struct Cycler {
    start: Vec<u64>,
    length: usize,
    sum: usize,
    // cycle: Vec<u64>,
}

fn cycler(start: &Vec<usize>, cycle: &Vec<usize>, mut pos: usize) -> usize {
    if pos < start.len() {
        return start[pos] as usize;
    }
    // println!("{:?}", start);
    let start_size: usize = *start.last().unwrap();
    pos -= start.len();
    let num_cycles = pos / CYCLE_LENGTH;
    let pos_cycle = pos % CYCLE_LENGTH;
    println!("{} + {} + {} * {}", start_size, cycle[..pos_cycle].iter().sum::<usize>(), num_cycles, cycle.iter().sum::<usize>());
    let res: usize = start_size + cycle[..pos_cycle].iter().sum::<usize>() + num_cycles * cycle.iter().sum::<usize>();

    res
}

fn read() -> Vec<bool> {
    include_str!("./../../data/17t.txt")
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

// fn main() {
//     for x in (0..100).rev() {
//         println!("{}", x);
//         let mut tetris = Tetris::new(read());
//         tetris.find_cycle(x, 10);
//     }
    
// }

fn main() {
    let mut tetris = Tetris::new(read());
    let cycle = tetris.find_cycle(STEPS_UNTIL_CYCLE, 2).into_iter().map(|x| x as usize).collect();

    let mut tetris = Tetris::new(read());
    for _ in 0..STEPS_UNTIL_CYCLE {
        tetris.fall();
    }
    let mut start = tetris.peaks;
    start[0] = 0;
    let start = start.iter().map(|&x| x as usize).collect();

    
    let mut tetris = Tetris::new(read());
    for _ in 0..1000 {
        tetris.fall();
    }
    tetris.peaks[0] = 0;
    for (k, val) in tetris.peaks.iter().enumerate() {
        let res = cycler(&start, &cycle, k);
        println!("{} : {}", k, res);
        assert_eq!(*val as usize, res);
    }

    // for k in 0.. 100 {
    //     let res = cycler(&start, &cycle, k);
    //     println!("{}", res);
    // }

    // let a = 1_000_000_000_000;
    // for x in a - 10000..a + 100 {
    //     let res = cycler(&start, &cycle, x);
    //     println!("{}", res);
    //     if res == 1514285714288 {
    //         println!("WOW {}", x);
    //         break;
    //     }
    // }
    
}



// fn main() {
//     println!("Hello, world!");
//     let mut tetris = Tetris::new(read());

//     println!("{:?}", tetris.input);
//     // for _ in 0..2022 {
//         // tetris.fall();
//         // println!("{:?}", tetris);
//     // }

//     let cycle = tetris.find_cycle(97, 2);

//     let mut tetris = Tetris::new(read());
//     for _ in 0..97 {
//         tetris.fall();
//     }

//     let mut peaks = tetris.peaks;
//     for x in cycle.iter() {
//         let prev = peaks.last().unwrap();
//         peaks.push(prev + *x as isize);
//     }

//     fn val(minutes: u64, peaks: &Vec<isize>, cycle_sum: u64) -> u64 {
//         // if minutes <= STEPS_UNTIL_CYCLE { return peaks[minutes as usize] as u64 };
//         let num_cycles = (minutes - STEPS_UNTIL_CYCLE) / CYCLE_LENGTH;
//         let mut pos = (minutes % CYCLE_LENGTH);
//         // if pos <= STEPS_UNTIL_CYCLE { pos += CYCLE_LENGTH }; 
//         println!("{} {}", num_cycles, pos);

//         let res = peaks[STEPS_UNTIL_CYCLE as usize + pos  as usize] as u64 + num_cycles * cycle_sum;
//         res
//     }

//     let cycle_sum: u64 = cycle.iter().map(|&x| x as u64).sum();

//     // let minutes: u64 = 10e12 as u64;
//     // let minutes: u64 = (1813 - 99) as u64; // 1813
//     // let minutes: u64 = (1813 - 98) as u64; // 1813
//     let minutes: u64 = (1715 + 97 + 1) as u64; // 1813

//     // let num_cycles = (minutes - STEPS_UNTIL_CYCLE) / CYCLE_LENGTH;
//     // let pos = (minutes % CYCLE_LENGTH);

//     // let res = peaks[pos as usize] as u64 + num_cycles * cycle_sum;

//     let mut peakc = Tetris::new(read());
//     for _ in 0..10000 { peakc.fall(); }
//     let peakc = peakc.peaks;
//     peaks[0] = 0;

//     for x in 97.. {
//         let a = val(x, &peaks, cycle_sum);
//         let b = peakc[x as usize] as u64;
//         println!("x : {}", x);
//         assert_eq!(a, b);
//     }

//     for minutes in minutes..minutes+100 {
//         let res = val(minutes, &peaks, cycle_sum);


//         println!("RESULT : {}", res);
        
//     }
//     // println!("RESULT : {}", peaks.last().unwrap());
//     // println!("COMPARE: {}", peaks[minutes as usize]);
//     return;

//     // {    let mut tetris = Tetris::new(read());

//     //     for _ in 0..10000 {
//     //         tetris.fall();
//     //     }
//     //     let peaks = tetris.peaks;

//     //     let mut tetris = Tetris::new(read());
//     //     for _ in 0..97 {
//     //         tetris.fall();
//     //     }
//     //     let mut start_peaks = tetris.peaks;
    
//     //     for x in cycle.iter() {
//     //         let prev = start_peaks.last().unwrap();
//     //         start_peaks.push(prev + *x as isize);
//     //     }
//     //     for x in cycle.iter() { let prev = start_peaks.last().unwrap(); start_peaks.push(prev + *x as isize); }
//     //     for x in cycle.iter() { let prev = start_peaks.last().unwrap(); start_peaks.push(prev + *x as isize); }
//     //     for x in cycle.iter() { let prev = start_peaks.last().unwrap(); start_peaks.push(prev + *x as isize); }
//     //     for x in cycle.iter() { let prev = start_peaks.last().unwrap(); start_peaks.push(prev + *x as isize); }
//     //     for x in cycle.iter() { let prev = start_peaks.last().unwrap(); start_peaks.push(prev + *x as isize); }

//     //     println!("{:?}", &peaks[..10000]);
//     //     println!("{:?}", &start_peaks[..10000]);

//     //     assert_eq!(&peaks[..10000], &start_peaks[..10000]);
//     // }

//     // println!("Height: {}", tetris.peaks.last().unwrap() + 1);
// }

