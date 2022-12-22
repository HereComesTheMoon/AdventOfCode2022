#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <tuple>

const std::string INPUT_FILE = "./data/22.txt";

typedef std::vector<std::vector<char>> Map;

enum Dir { Right, Down, Left, Up };

class Player {
public:
  Map m;
  uint x;
  uint y;
  Dir dir = Right;

  Player(const Map& tm) {
    m = tm;
    auto start = std::find(m[0].begin(), m[0].end(), '.');
    if (start == m[0].end()) assert(false);
    x = start - m[0].begin();
    y = 0;
    assert(m.at(y).at(x) == '.');
    assert(m.at(y).at(x-1) == ' ');
  }

  std::tuple<uint, uint> next_tile() const {
    assert(m.at(y).at(x) == '.');
    uint i = x;
    uint j = y;
    switch (dir) {
      case Right:
        while (true) {
          ++i;
          if (m.at(y).size() <= i) i = 0;
          if (m[y][i] == ' ') continue;
          return {i, y};
        }
      case Down:
        while (true) {
          ++j;
          if (m.size() <= j) j = 0;
          if (m[j][x] == ' ') continue;
          return {x, j};
        }
      case Left:
        while (true) {
          if (i == 0) i = m.at(y).size();
          --i;
          if (m[y][i] == ' ') continue;
          return {i, y};
        }
      case Up:
        while (true) {
          if (j == 0) j = m.size();
          --j;
          if (m[j][x] == ' ') continue;
          return {x, j};
        }
      default: assert(false);
    }
    assert(false);
  }

  void step(uint times) {
    for (uint i = 0; i < times; ++i) {
      auto [xx, yy] = next_tile();
      if (m.at(yy).at(xx) == '#') {
        return;
      }
      x = xx;
      y = yy;
    }
  }

  void turn(char turn) {
    assert(turn == 'L' || turn == 'R');
    switch (dir) {
    case Right:
      dir = (turn == 'L') ? Up : Down; break;
    case Down:
      dir = (turn == 'L') ? Right : Left; break;
    case Left:
      dir = (turn == 'L') ? Down : Up; break;
    case Up:
      dir = (turn == 'L') ? Left : Right; break;
    default: assert(false);
    }
  }

  void walk(const std::string& directions) {
    uint i = 0;
    uint j = 0;
    while (j < directions.size()) { 
      if (directions[j] == 'L' || directions[j] == 'R') {
        if (i < j) {
          step(std::stoi(directions.substr(i, j - i)));
        }
        turn(directions[j]);
        ++j;
        i = j;
      } else {
        ++j;
      }
    }
    if (i < j) {
      step(std::stoi(directions.substr(i, j - i)));
    }
  }

  void print() const {
    std::cout << std::endl;
    for (uint j = 0; j < m.size(); ++j) {
      for (uint i = 0; i < m.at(j).size(); ++i) {
        if (i == x && j == y) {
          std::cout << "X";
        } else {
          std::cout << m[j][i];
        }
      }
      std::cout << std::endl;
    }
  }
};

std::tuple<Map, std::string> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;
  
  std::vector<std::vector<char>> map;
  uint counter = 0;
  size_t max_line = 0;
  while (std::getline(infile, line)) {
    if (line.size() == 0) {
      break;
    }
    max_line = std::max(max_line, line.size());
    map.emplace_back(std::vector<char>(line.begin(), line.end()));
    for (const auto& c : line) {
      if (c == '.' or c == '#') ++counter;
    }
  }
  int width = sqrt(counter / 6);
  printf("Number tiles: %d. Number tiles / 6: %d. The square root of that is the width of each side: %d\n",
     counter, counter / 6, width);
  assert(counter % 6 == 0);
  assert(width * width * 6 == counter);
  std::getline(infile, line);

  for (auto& row : map) {
    row.resize(max_line, ' ');
  }

  return {map, line};
}

int main() {
  auto [m, dirs] = read();

  auto p = Player(m);

  p.walk(dirs);

  auto res = 1000 * (p.y + 1) + 4 * (p.x + 1) + p.dir;

  std::cout << "Result: " << res << std::endl;
  return 0;
}
