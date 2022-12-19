use std::collections::HashSet;

const DIRS: [(i32, i32, i32); 6] = [
    (1,0,0),
    (-1,0,0),
    (0,1,0),
    (0,-1,0),
    (0,0,1),
    (0,0,-1),
];

struct Droplet(HashSet<(i32, i32, i32)>);

fn read() -> Droplet {
    let inp = include_str!("../../data/18.txt")
        .lines()
        .map(|row| row.split(',')
            .map(|x| x.parse().unwrap())
            .collect::<Vec<i32>>()
        );

    let inp = inp
        .map(|row| match row[..] {
            [a,b,c] => { (a, b, c) },
            _ => unreachable!(),
        });


    let data = HashSet::from_iter(inp);
    Droplet(data)
}

impl Droplet {
    fn surface(&self) -> u32 {
        let mut seen = HashSet::with_capacity(self.0.len());

        let mut count = 0;

        for (x, y, z) in self.0.iter() {
            if seen.contains(&(x,y,z)) { continue; }
            seen.insert((x, y, z));
            for (a, b, c) in DIRS {
                if !self.0.contains(&(x + a, y + b, z + c)) {
                    count += 1;
                }
            }
        }
        
        println!("Surface area: {}", count);
        count
    }

    fn outer_surface(&self) -> u32 {
        let (mut x_min, mut x_max) = (i32::MAX, i32::MIN);
        let (mut y_min, mut y_max) = (i32::MAX, i32::MIN);
        let (mut z_min, mut z_max) = (i32::MAX, i32::MIN);

        for (x, y, z) in self.0.iter() {
            x_min = x_min.min(*x);
            y_min = y_min.min(*y);
            z_min = z_min.min(*z);

            x_max = x_max.max(*x);
            y_max = y_max.max(*y);
            z_max = z_max.max(*z);
        }
        x_min = x_min - 1;
        y_min = y_min - 1;
        z_min = z_min - 1;

        x_max = x_max + 1;
        y_max = y_max + 1;
        z_max = z_max + 1;


        let mut stack = vec![(
            (x_min - 1), 
            (y_min - 1), 
            (z_min - 1), 
        )];

        let mut boxed_droplet = Droplet(self.0.clone());

        while let Some((x, y, z)) = stack.pop() {
            boxed_droplet.0.insert((x, y, z));
            for (a, b, c) in DIRS {
                let (xx, yy, zz) = (x + a, y + b, z + c);
                if boxed_droplet.0.contains(&(xx, yy, zz)) { continue; }
                if xx < x_min - 1 || x_max < xx + 1 { continue; }
                if yy < y_min - 1 || y_max < yy + 1 { continue; }
                if zz < z_min - 1 || z_max < zz + 1 { continue; }
                stack.push((xx, yy, zz));
                boxed_droplet.0.insert((xx, yy, zz));
            }
        }

        { // draw
            for z in z_min..z_max {
                println!();
                println!("Layer {}:", z);
                for y in y_min..y_max {
                    println!();
                    for x in x_min..x_max {
                        if boxed_droplet.0.contains(&(x,y,z)) {
                            if self.0.contains(&(x,y,z)) {
                                print!("\x1b[93mO\x1b[0m");
                            } else {
                                print!("X");
                            }
                        } else {
                            print!(".");
                        }
                    }
                }
            }
        }

        // surface(boxed_droplet) = encased_surface + box_surface
        // surface(self) = goal + encased_surface
        let (dx, dy, dz) = ((x_max - x_min + 1), (z_max - z_min + 1), (y_max - y_min + 1));
        let boxed_surface = 2*(dx*dy + dy*dz + dz*dx) as u32;
        let res = self.surface() + boxed_surface - boxed_droplet.surface();
        println!("Outer surface: {}", res);
        res
    }
}


fn main() {
    println!("Hello, world!");

    let d = read();

    d.outer_surface();

}

