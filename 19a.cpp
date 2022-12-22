#include <array>
#include <assert.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <stdint.h>
#include <vector>

const std::string INPUT_FILE = "./data/19.txt";
const uint MINUTES = 24;

// Ore, Clay, Obsidian, Geode
typedef std::array<uint, 4> Resources;

class Blueprint {
public:
  int id;
  std::array<Resources, 4> costs;

  void print() const {
    printf("Id: %d. Cost Ore      robot: {%d,%d,%d,%d}\n", id, costs[0][0],
           costs[0][1], costs[0][2], costs[0][3]);
    printf("Id: %d. Cost Clay     robot: {%d,%d,%d,%d}\n", id, costs[1][0],
           costs[1][1], costs[1][2], costs[1][3]);
    printf("Id: %d. Cost Obsidian robot: {%d,%d,%d,%d}\n", id, costs[2][0],
           costs[2][1], costs[2][2], costs[2][3]);
    printf("Id: %d. Cost Geode    robot: {%d,%d,%d,%d}\n", id, costs[3][0],
           costs[3][1], costs[3][2], costs[3][3]);
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

std::vector<State> branch(const Blueprint &b, const State &s) {
  auto branches =
      std::vector<State>({State{s.min + 1,
                                {s.ores[0] + s.bots[0], s.ores[1] + s.bots[1],
                                 s.ores[2] + s.bots[2], s.ores[3] + s.bots[3]},
                                s.bots}});

  for (uint i = 0; i < 4; ++i) {
    auto cost = b.costs[i];
    if (can_afford(cost, s)) {
      auto new_bot = s.bots;
      ++new_bot[i];
      branches.emplace_back(State{
          s.min + 1,
          {s.ores[0] + s.bots[0] - cost[0], s.ores[1] + s.bots[1] - cost[1],
           s.ores[2] + s.bots[2] - cost[2], s.ores[3] + s.bots[3] - cost[3]},
          new_bot,
      });
    }
  }
  return branches;
}

uint bfs(const Blueprint &b) {
  auto q = std::deque<State>({State{0, {{0, 0, 0, 0}}, {{1, 0, 0, 0}}}});

  auto seen = std::set<std::pair<Resources, Resources>>();

  // std::array<uint, MINUTES> bots_at_min{};
  std::array<uint, MINUTES + 1> geodes_at_min{};
  // uint max_geodes = 0;
  uint max_minute = 0;

  while (!q.empty()) {
    auto node = q.front();
    if (node.min > max_minute) {
      std::cout << "Minute: " << node.min << std::endl;
      max_minute = node.min;
    }
    // print(node);
    q.pop_front();
    seen.insert({node.ores, node.bots});
    // std::cout << "Children:" << std::endl;
    for (const auto &s : branch(b, node)) {
      geodes_at_min.at(s.min) = std::max(geodes_at_min[s.min], s.ores[3]);
      // auto number_bots = s.bots[0] + s.bots[1] + s.bots[2] + s.bots[3];
      // bots_at_min[s.min] = std::max(bots_at_min[s.min], number_bots);

      if (s.ores[3] + 0 < geodes_at_min[s.min]) continue;

      // if (number_bots + 2 < bots_at_min[s.min] || s.ores[3] + 2 < geodes_at_min[s.min]) { continue; }
      if (seen.contains({s.ores, s.bots})) continue;
      if (s.min == MINUTES)
        continue;
      q.push_back(s);
    }
  }

  return geodes_at_min.back();
}

void simulate(const Blueprint& b) {
  auto s = State{0, {{0, 0, 0, 0}}, {{1, 0, 0, 0}}};
  int inp;
  print(s);
  while (s.min < MINUTES) {
    // bool valid = false;
    while (true) {
      inp = getchar();
      if (inp == '\n') continue;
      if (inp == '5') {
        s = State{s.min + 1, {s.ores[0] + s.bots[0],s.ores[1] + s.bots[1],s.ores[2] + s.bots[2],s.ores[3] + s.bots[3]}, s.bots};
        break;
      } else if ('1' <= inp && inp <= '4') {
        if (can_afford(b.costs[inp - '1'], s)) {
          auto new_bot = s.bots;
          new_bot[inp - '1']++;
          s = State{s.min + 1, {s.ores[0] - b.costs[inp - '1'][0] + s.bots[0],s.ores[1] - b.costs[inp - '1'][1] + s.bots[1],s.ores[2] - b.costs[inp - '1'][2] + s.bots[2],s.ores[3] - b.costs[inp - '1'][3] + s.bots[3]}, new_bot};
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
  while (std::getline(infile, line)) {
    auto begin = std::sregex_iterator(line.begin(), line.end(), numbers);
    auto end = std::sregex_iterator();
    auto vals = std::array<uint, 7>();

    uint count = 0;
    for (auto i = begin; i != end; ++i) {
      vals.at(count) = std::stoi(i->str());
      ++count;
    }
    data.emplace_back(Blueprint{int(vals[0]),
                                {{
                                    {vals[1], 0, 0, 0},
                                    {vals[2], 0, 0, 0},
                                    {vals[3], vals[4], 0, 0},
                                    {vals[5], 0, vals[6], 0},
                                }}});
  }
  return data;
}

uint check_quality(const std::vector<Blueprint>& data) {
  auto quality_sum = 0;
  for (const auto& b : data) {
    std::cout << "Checking blueprint " << b.id << std::endl;
    b.print();
    auto geodes = bfs(b);
    std::cout << "Geodes cracked open: " << geodes << std::endl;
    std::cout << "Quality level: " << geodes * b.id << std::endl;
    quality_sum += geodes * b.id;
  }

  std::cout << "Total quality: " << quality_sum << std::endl;
  return quality_sum;
}

int main() {
  auto data = read();

  // simulate(data[0]);

  // data[0].print();
  // data[1].print();
  // auto res = bfs(data[1]);
  check_quality(data);
  // std::cout << "Result: " << res << std::endl;
  return 0;
}
