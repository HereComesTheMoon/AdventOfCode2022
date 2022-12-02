fn main() {
    println!(
        "{}",
        include_str!("../../data/1.txt")
            .split("\n\n")
            .map(|elf| elf
                .lines()
                .map(|ration| ration.parse::<usize>().unwrap())
                .sum())
            .fold([0_usize; 4], |mut top, x| {
                top[0] = x;
                top.sort();
                top
            })[1..]
            .into_iter()
            .sum::<usize>()
    );
}

#[cfg(test)]
mod test {
    const N: usize = 100000;

    #[test]
    fn timsort() {
        println!(
            "{}",
            include_str!("../../data/1.txt")
                .repeat(N)
                .split("\n\n")
                .map(|elf| elf
                    .lines()
                    .map(|ration| ration.parse::<usize>().unwrap())
                    .sum())
                .fold([0_usize; 4], |mut top, x| {
                    top[0] = x;
                    top.sort();
                    top
                })[1..]
                .into_iter()
                .sum::<usize>()
        );
    }

    #[test]
    fn ifs() {
        println!(
            "{}",
            include_str!("../../data/1.txt")
                .repeat(N)
                .split("\n\n")
                .map(|elf| elf
                    .lines()
                    .map(|ration| ration.parse::<usize>().unwrap())
                    .sum())
                .fold([0_usize; 3], |mut top, x| {
                    if top[0] < x {
                        top[2] = top[1];
                        top[1] = top[0];
                        top[0] = x;
                        top
                    } else if top[1] < x {
                        top[2] = top[1];
                        top[1] = x;
                        top
                    } else if top[2] < x {
                        top[2] = x;
                        top
                    } else {
                        top
                    }
                })
                .into_iter()
                .sum::<usize>()
        );
    }
}
