#include <array>
#include <assert.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <vector>

const std::string INPUT_FILE = "./data/19.txt";
const uint MINUTES = 32;

// Ore, Clay, Obsidian, Geode
typedef std::array<uint, 4> Resources;

class Blueprint {
public:
  int id;
  std::array<Resources, 4> costs;

  void print() const {
    printf("Id: %d\n", id);
    printf("Cost Ore      robot: {%d,%d,%d,%d}\n", costs[0][0], costs[0][1], costs[0][2], costs[0][3]);
    printf("Cost Clay     robot: {%d,%d,%d,%d}\n", costs[1][0], costs[1][1], costs[1][2], costs[1][3]);
    printf("Cost Obsidian robot: {%d,%d,%d,%d}\n", costs[2][0], costs[2][1], costs[2][2], costs[2][3]);
    printf("Cost Geode    robot: {%d,%d,%d,%d}\n", costs[3][0], costs[3][1], costs[3][2], costs[3][3]);
    std::cout << std::endl;
  }
};

struct State {
  uint min;
  Resources ores;
  Resources bots;
};

void print(const State &s) {
  printf("Min: %d. Resources: {%d,%d,%d,%d} Bots: {%d,%d,%d,%d}", s.min,
         s.ores[0], s.ores[1], s.ores[2], s.ores[3], s.bots[0], s.bots[1],
         s.bots[2], s.bots[3]);
  std::cout << std::endl;
}

bool can_afford(const Resources &cost, const State &s) {
  for (uint j = 0; j < 4; ++j) {
    if (s.ores[j] < cost[j])
      return false;
  }
  return true;
}

State build(const Blueprint &b, const State &s, const int i) {
  if (i == -1) {
    return State{s.min + 1,
                 {s.ores[0] + s.bots[0], s.ores[1] + s.bots[1],
                  s.ores[2] + s.bots[2], s.ores[3] + s.bots[3]},
                 s.bots};
  }
  auto new_bot = s.bots;
  ++new_bot[i];
  return State{
      s.min + 1,
      {s.ores[0] + s.bots[0] - b.costs[i][0],
       s.ores[1] + s.bots[1] - b.costs[i][1],
       s.ores[2] + s.bots[2] - b.costs[i][2],
       s.ores[3] + s.bots[3] - b.costs[i][3]},
      new_bot,
  };
}

std::vector<State> branch(const Blueprint &b, const State &s) {
  if (can_afford(b.costs[3], s)) {
    return {{build(b, s, 3)}};
  }
  auto branches = std::vector<State>({build(b, s, -1)});

  auto cost = b.costs[2];
  if (can_afford(cost, s))
    branches.emplace_back(build(b, s, 2));
  cost = b.costs[1];
  if (can_afford(cost, s) && s.bots[1] < std::max({b.costs[0][1], b.costs[1][1], b.costs[2][1], b.costs[3][1]}))
    branches.emplace_back(build(b, s, 1));
  cost = b.costs[0];
  if (can_afford(cost, s) && s.bots[0] < std::max({b.costs[0][0], b.costs[1][0], b.costs[2][0], b.costs[3][0]}))
    branches.emplace_back(build(b, s, 0));

  return branches;
}

uint bfs(const Blueprint &b) {
  auto q = std::deque<State>({State{0, {{0, 0, 0, 0}}, {{1, 0, 0, 0}}}});

  auto seen = std::set<std::pair<Resources, Resources>>();

  std::array<uint, MINUTES + 1> geodes_at_min{};
  uint max_minute = 0;

  while (!q.empty()) {
    auto node = q.front();
    if (node.min > max_minute) {
      std::cout << "Minute: " << node.min << std::endl;
      max_minute = node.min;
    }
    q.pop_front();
    seen.insert({node.ores, node.bots});
    for (const auto &s : branch(b, node)) {
      geodes_at_min.at(s.min) = std::max(geodes_at_min[s.min], s.ores[3]);

      if (s.ores[3] + 1 < geodes_at_min[s.min])
        continue;

      if (seen.contains({s.ores, s.bots}))
        continue;
      if (s.min == MINUTES)
        continue;
      q.push_back(s);
    }
  }

  return geodes_at_min.back();
}

void simulate(const Blueprint &b) {
  auto s = State{0, {{0, 0, 0, 0}}, {{1, 0, 0, 0}}};
  int inp;
  print(s);
  while (s.min < MINUTES) {
    while (true) {
      inp = getchar();
      if (inp == '\n')
        continue;
      if (inp == '5') {
        s = build(b, s, -1);
        break;
      } else if ('1' <= inp && inp <= '4') {
        if (can_afford(b.costs[inp - '1'], s)) {
          s = build(b, s, inp - '1');
          break;
        } else {
          std::cout << "Can't afford!" << std::endl;
        }
      } else {
        std::cout << "Invalid input! Try 1,2,3,4,5" << std::endl;
      }
    }
    print(s);
  }
}

std::vector<Blueprint> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<Blueprint>();

  std::regex numbers("-?\\d+");
  for (uint i = 0; i < 3; ++i) {
    std::getline(infile, line);
    auto begin = std::sregex_iterator(line.begin(), line.end(), numbers);
    auto end = std::sregex_iterator();
    auto vals = std::array<uint, 7>();

    uint count = 0;
    for (auto i = begin; i != end; ++i) {
      vals.at(count) = std::stoi(i->str());
      ++count;
    }
    auto bp = Blueprint{int(vals[0]),
                        {{
                            {vals[1], 0, 0, 0},
                            {vals[2], 0, 0, 0},
                            {vals[3], vals[4], 0, 0},
                            {vals[5], 0, vals[6], 0},
                        }}};
    data.emplace_back(bp);
  }
  return data;
}

uint crack_geodes(const std::vector<Blueprint> &data) {
  auto res = 1;
  for (const auto &b : data) {
    std::cout << "Checking blueprint " << b.id << std::endl;
    b.print();
    auto geodes = bfs(b);
    std::cout << "Geodes cracked open: " << geodes << std::endl;
    res *= geodes;
  }

  std::cout << "Total quality: " << res << std::endl;
  return res;
}

int main() {
  auto data = read();

  crack_geodes(data);
  return 0;
}
