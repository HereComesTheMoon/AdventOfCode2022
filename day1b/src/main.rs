fn main() {
    println!("{}", include_str!("../../data/1.txt")
        .split("\n\n")
        .map(|elf| elf
            .lines()
            .map(|ration| ration
                .parse::<usize>()
                .unwrap())
            .sum())
        .fold([0_usize; 4], |mut top, x| {
            top[0] = x;
            top.sort();
            top
        })[1..]
        .into_iter()
        .sum::<usize>());
}
