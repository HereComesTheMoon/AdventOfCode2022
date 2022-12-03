use std::collections::HashSet;

fn main() {
    let mut sum_of_priorities = 0;
    let data = include_str!("../../data/3.txt");
    // let data = "vJrwpWtwJgWrhcsFMMfFFhFp\njqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\nPmmdzqPrVvPwwTWBwg\nwMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\nttgJtRGJQctTZtZT\nCrZsJsPPZsGzwwsLwLmpwMDw";
    let data: Vec<_> = data.lines().collect();
    for k in (0..data.len()).step_by(3) {
        let a: HashSet<char> = HashSet::from_iter(data[k].chars());
        let b: HashSet<char> = HashSet::from_iter(data[k+1].chars());
        let c: HashSet<char> = HashSet::from_iter(data[k+2].chars());

        for &x in a.intersection(&b) {
            if c.contains(&x) {
                sum_of_priorities += match x as u8 {
                    b'a'..=b'z' => 1 + x as u8 - b'a',
                    b'A'..=b'Z' => 27 + x as u8 - b'A',
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
