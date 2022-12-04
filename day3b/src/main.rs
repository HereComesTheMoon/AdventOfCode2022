use std::collections::HashSet;

fn main() {
    let mut sum_of_priorities = 0;

    let data: Vec<HashSet<&u8>> = include_str!("../../data/3.txt")
        .lines()
        .map(|row| HashSet::from_iter(row.as_bytes()))
        .collect();

    for bags in data.chunks_exact(3) {
        for &x in bags[0].intersection(&bags[1]) {
            if bags[2].contains(x) {
                sum_of_priorities += match x {
                    b'a'..=b'z' => 1 + x - b'a',
                    b'A'..=b'Z' => 27 + x - b'A',
                    _ => {
                        unreachable!()
                    }
                } as usize;
                break;
            }
            
        }
    }

    println!("{}", sum_of_priorities);
}
