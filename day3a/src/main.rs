use std::collections::HashSet;

fn main() {
    let mut sum_of_priorities = 0;
    let data = include_str!("../../data/3.txt").lines().map(|row| row.as_bytes().split_at(row.len() / 2));
    for (head, tail) in data {
        let a: HashSet<&u8> = HashSet::from_iter(head);
        for x in tail {
            if a.contains(x) {
                sum_of_priorities += match x {
                    b'a'..=b'z' => { 1 + x - b'a' },
                    b'A'..=b'Z' => { 27 + x - b'A' },
                    _ => { unreachable!() },
                } as usize;
                break;
            }
        }
    }
    println!("{}", sum_of_priorities);
}


