use std::collections::HashSet;

fn main() {
    let mut sum_of_priorities = 0;
    let data = include_str!("../../data/3.txt");
    // let data = "vJrwpWtwJgWrhcsFMMfFFhFp\njqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\nPmmdzqPrVvPwwTWBwg\nwMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\nttgJtRGJQctTZtZT\nCrZsJsPPZsGzwwsLwLmpwMDw";
    let data = data.lines().map(|row| row.split_at(row.len() / 2));
    for (head, tail) in data {
        let a: HashSet<char> = HashSet::from_iter(head.chars());
        let b = HashSet::from_iter(tail.chars());
        let mut res = HashSet::intersection(&a, &b);
        let item = *res.next().unwrap() as u8;
        println!("{}", sum_of_priorities);
        println!("{}", item as char);
        sum_of_priorities += match item {
            b'a'..=b'z' => { 1 + item - b'a' },
            b'A'..=b'Z' => { 27 + item - b'A' },
            _ => { unreachable!() },
        } as usize
    }
    println!("{}", sum_of_priorities);
}


