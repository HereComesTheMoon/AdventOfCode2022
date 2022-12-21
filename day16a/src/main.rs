use std::collections::HashMap;
use std::collections::HashSet;
use std::collections::VecDeque;

const MINUTES: u32 = 30;

struct WeightedCompleteGraph<'a> {
    nodes: Vec<&'a str>,
    flows: HashMap<&'a str, u32>,
    dists: HashMap<(&'a str, &'a str), u32>,
}

impl<'a> WeightedCompleteGraph<'a> {
    fn new(nodes: Vec<&'a str>, flows: HashMap<&'a str, u32>, dists: HashMap<(&'a str, &'a str), u32>) -> Self {
        WeightedCompleteGraph { nodes, flows, dists }
    }

    fn brute_force_better(&self, path: &mut Vec<&'a str>, minutes_left: u32) -> u32 {
        let mut best_path = 0;
        for x in self.nodes.iter() {
            if self.flows[x] == 0 { continue; }    
            if path.contains(x) { continue; }
            let d = self.dists[&(*path.last().unwrap(), *x)] + 1;
            if minutes_left <= d { continue; }
            path.push(x);
            let rec = self.brute_force_better(path, minutes_left - d);
            path.pop();
            best_path = best_path.max(self.flows[x] * (minutes_left - d) + rec);
        }
        return best_path;
    }
}


struct Graph<'a> {
    nodes: Vec<&'a str>,
    edges: HashMap<&'a str, Vec<&'a str>>,
    flows: HashMap<&'a str, u32>,
    dists: HashMap<(&'a str, &'a str), u32>,
}

impl Graph<'_> {
    fn new(data: Vec<(&'static str, u32, Vec<&'static str>)>) -> Self {
        let mut g = Graph{
            nodes: vec![],
            edges: HashMap::new(),
            flows: HashMap::new(),
            dists: HashMap::new(),
            
        };
        for (name, flow, neighbours) in data {
            g.nodes.push(name);
            g.flows.insert(name, flow);
            g.edges.insert(name, neighbours);
        }
        g
    }

    fn find_all_shortest_paths(&mut self) {
        for a in self.nodes.iter() {
            if self.flows[a] == 0 && a != &"AA" { continue; }
            for b in self.nodes.iter() {
                if self.flows[b] == 0 { continue; }
                if a == b { continue; }
                let d = self.shortest_path(a, b);
                self.dists.insert((a, b), d);
            }
        }
    }

    fn shortest_path(&self, start: &str, goal: &str) -> u32 {
        assert!(self.flows.contains_key(start));
        assert!(self.flows.contains_key(goal));
        if start == goal {
            return 0;
        }

        let mut q = VecDeque::from_iter(self.edges[start].iter().map(|n| (n, 1)));
        let mut seen = HashSet::new();
        seen.insert(start);

        while !q.is_empty() {
            let (&val, dist) = q.pop_front().unwrap();
            if val == goal {
                return dist;
            }
            let neighbours = &self.edges[val];
            for n in neighbours {
                if !seen.contains(n) {
                    q.push_back((&n, dist + 1));
                    seen.insert(&n);
                } 
            }
        }
        unreachable!()
    }
}

fn read() -> Vec<(&'static str, u32, Vec<&'static str>)> {
    let input = include_str!("../../data/16.txt").lines();

    let mut data = vec![];

    for line in input {
        let valve = &line[6..8];
        let flow = line.split_once('=').unwrap().1.split_once(';').unwrap().0.parse().unwrap();
        let adjacent: Vec<&str> = line.split_once("to valve").unwrap().1[1..].trim_start().split(", ").collect();

        data.push((valve, flow, adjacent));
    }
    data
}

fn main() {
    println!("Hello, world!");
    let data = read();

    let mut g = Graph::new(data);
    g.find_all_shortest_paths();

    let wcg = WeightedCompleteGraph::new(g.nodes, g.flows, g.dists);

    let res = wcg.brute_force_better(&mut vec!["AA"], MINUTES);

    println!("Result: {}", res);
}
