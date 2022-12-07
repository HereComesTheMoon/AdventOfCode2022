fn main() {
    println!("Hello, world!");

    let inpt = include_str!("./../../data/7t.sh");
    let input = parse(&inpt);

    for x in input {
        println!("{:?}", x);
    }
    // println!("{:?}", input);

}

#[derive(Debug)]
enum File {
    Dir(String),
    Bin(usize, String)
}

#[derive(Debug)]
enum Command {
    LS(Vec<File>),
    CD(String),
}

fn parse(input: &'static str) -> impl Iterator<Item = Command> {
    fn parse_file(f: &str) -> File {
        let (a, b) = f.split_once(char::is_whitespace).unwrap();
        if a.starts_with("dir") {
            File::Dir(b.to_owned())
        } else {
            File::Bin(a.parse().unwrap(), b.to_owned())
        }
    }

    input
        .trim()
        .split("$ ")
        .skip(1)
        .map(|chunk|  chunk.split_at(2) )
        // .map(|x| {println!("{:?}", x); x} )
        .map(|(cmd, rest)| match cmd {
            "cd" => { Command::CD(rest.trim().to_owned()) },
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

fn part1(input: &'static str) {
    let it = parse(input);
    let mut stack = vec![];

    for cmd in it {
        match cmd {
            Command::LS(v) => {},
            Command::CD("..") => {},
            Command::CD(dir) => {},
        }
    }
}






// "$ cd /
// $ ls
// dir a
// 14848514 b.txt
// 8504156 c.dat
// dir d
// $ cd a
// $ ls
// dir e
// 29116 f
// 2557 g
// 62596 h.lst
// $ cd e
// $ ls
// 584 i
// $ cd ..
// $ cd ..
// $ cd d
// $ ls
// 4060174 j
// 8033020 d.log
// 5626152 d.ext
// 7214296 k"
