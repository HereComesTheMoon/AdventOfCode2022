fn main() {
    let res: usize = include_str!("../../data/1.txt")
        .split("\n\n")
        .map(|elf| elf
            .lines()
            .map(|ration| ration
                .parse::<usize>()
                .unwrap())
            .sum())
        .max()
        .unwrap();
    

    println!("{}", res);
}
