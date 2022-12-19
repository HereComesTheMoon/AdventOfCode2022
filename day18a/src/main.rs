use std::collections::HashSet;

const DIRS: [(i32, i32, i32); 6] = [
    (1,0,0),
    (-1,0,0),
    (0,1,0),
    (0,-1,0),
    (0,0,1),
    (0,0,-1),
];

struct Droplet(HashSet<(i32, i32, i32)>);

fn read() -> Droplet {
    let inp = include_str!("../../data/18.txt")
        .lines()
        .map(|row| row.split(',')
            .map(|x| x.parse().unwrap())
            .collect::<Vec<i32>>()
        );

    let inp = inp
        .map(|row| match row[..] {
            [a,b,c] => { (a, b, c) },
            _ => unreachable!(),
        });


    let data = HashSet::from_iter(inp);
    Droplet(data)
}

impl Droplet {
    fn surface(&self) -> u32 {
        let mut seen = HashSet::with_capacity(self.0.len());

        let mut count = 0;

        for (x, y, z) in self.0.iter() {
            if seen.contains(&(x,y,z)) { continue; }
            seen.insert((x, y, z));
            for (a, b, c) in DIRS {
                if !self.0.contains(&(x + a, y + b, z + c)) {
                    count += 1;
                }
            }
        }
        
        println!("Surface area: {}", count);
        count
    }
}

fn main() {
    println!("Hello, world!");

    let d = read();

    d.surface();

}

