use std::collections::HashSet;

fn main() {
    let mut sum_of_priorities = 0;
    let data: Vec<_> = include_str!("../../data/3.txt").lines().collect();
    for k in (0..data.len()).step_by(3) {
        let a: HashSet<&u8> = HashSet::from_iter(data[k].as_bytes());
        let b: HashSet<&u8> = HashSet::from_iter(data[k+1].as_bytes());
        let c: HashSet<&u8> = HashSet::from_iter(data[k+2].as_bytes());

        for &x in a.intersection(&b) {
            if c.contains(&x) {
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
