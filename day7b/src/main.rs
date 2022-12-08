use std::collections::HashMap;

fn main() {
    println!("Hello, world!");

    let inpt = include_str!("./../../data/7.txt");

    part2(inpt);
}

const TOTAL_SPACE: u32 = 70_000_000;
const NEEDED_SPACE: u32 = 30_000_000;

#[derive(Debug)]
enum File<'a> {
    Dir(&'a str),
    Bin(u32, &'a str)
}

#[derive(Debug)]
enum Command<'a> {
    LS(Vec<File<'a>>),
    CD(&'a str),
}

fn parse<'a>(input: &'a str) -> impl Iterator<Item = Command> {
    fn parse_file(f: &str) -> File {
        let (a, b) = f.split_once(char::is_whitespace).unwrap();
        if a.starts_with("dir") {
            File::Dir(b)
        } else {
            File::Bin(a.parse().unwrap(), b)
        }
    }

    input
        .trim()
        .split("$ ")
        .skip(1)
        .map(|chunk|  chunk.split_at(2) )
        .map(|(cmd, rest)| match cmd {
            "cd" => { Command::CD(rest.trim()) },
            "ls" => { Command::LS(
                rest
                    .trim()
                    .lines()
                    .map(|row| parse_file(row))
                    .collect()
            ) },
            _ => unreachable!()
        })
}

fn part2(input: &'static str) -> u32 {
    let it = parse(input);
    let mut parents = vec![];

    let mut sizes:HashMap<Vec<&str>, u32> = HashMap::new();
    
    for cmd in it {
        match cmd {
            Command::LS(files) => {
                for file in files {
                    if let File::Bin(size, _) = file {
                        *sizes.get_mut(&parents).unwrap() += size;
                    }
                }
            },
            Command::CD("..") => {
                let size = *sizes.get(&parents).unwrap();
                parents.pop();
                *sizes.get_mut(&parents).unwrap() += size;
            },
            Command::CD(dir) => {
                parents.push(dir);
                sizes.insert(parents.clone(), 0);
            },
        }
    }
    while 2 <= parents.len() {
        let size = *sizes.get(&parents).unwrap();
        parents.pop();
        *sizes.get_mut(&parents).unwrap() += size;
    }

    let size_needed: u32 = NEEDED_SPACE - (TOTAL_SPACE - *sizes.get(&vec!["/"]).unwrap());
        
    let res = sizes
        .into_values()
        .filter(|x| &size_needed <= x)
        .min()
        .unwrap();

    println!("The result is: {}", res);
    res
}
