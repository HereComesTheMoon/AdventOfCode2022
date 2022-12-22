#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <array>

const std::string INPUT_FILE = "./data/22.txt";
const uint WIDTH = 50;

typedef std::array<std::array<char, WIDTH>, WIDTH> Side;
typedef std::array<Side, 6> Map;
typedef std::array<std::array<std::pair<char, char>, 4>, 6> Edges;

const std::array<std::pair<uint, uint>, 6> pos_side = {{
    {1*50, 0*50},
    {2*50, 0*50},
    {1*50, 1*50},
    {0*50, 3*50},
    {0*50, 2*50},
    {1*50, 2*50},
  }};


enum Dir { Right, Down, Left, Up };

Side side_from_map(std::vector<std::vector<char>>& m, uint x, uint y) {
  assert(m.at(y).at(x) != ' ');
  Side side;
  for (uint j = 0; j < WIDTH; ++j) {
    for (uint i = 0; i <  WIDTH; ++i) {
      side.at(j).at(i) = m.at(y + j).at(x + i);
    }
  }
  return side;
}

class Player {
public:
  Map m;
  Edges edges;
  uint s = 0;
  uint x = 0;
  uint y = 0;
  Dir dir = Right;

  Player(Map tm) {
    m = tm;
    assert(m[s].at(y).at(x) == '.');

    edges = { {
      {{ {2, 0}, {3, 0}, {5, 2}, {4, 1} }},
      {{ {6, 2}, {3, 1}, {1, 0}, {4, 0} }},
      {{ {2, 3}, {6, 0}, {5, 3}, {1, 0} }},
      {{ {6, 3}, {2, 0}, {1, 3}, {5, 0} }},
      {{ {6, 0}, {4, 0}, {1, 2}, {3, 1} }},
      {{ {2, 2}, {4, 1}, {5, 0}, {3, 0} }},
    } };
  }

  std::tuple<uint, uint, Dir> rotate(uint x, uint y, Dir d, uint number) const {
    for (uint i = 0; i < number; ++i) {
      uint xx = x;
      x = WIDTH - 1 - y;
      y = xx;

      switch (d) {
      case Right: d = Down ; break;
      case Down:  d = Left ; break;
      case Left:  d = Up   ; break;
      case Up:    d = Right; break;
      default: assert(false);
      }
    }
    return {x, y, d};
  }

  std::tuple<uint, uint, uint, Dir> next_tile() const {
    assert(m[s].at(y).at(x) == '.');
    uint xx = x;
    uint yy = y;
    switch (dir) {
    case Right:
      if (x < WIDTH - 1) return {s, x + 1, y    , dir};
      xx = 0;
      break;
    case Down:
      if (y < WIDTH - 1) return {s, x    , y + 1, dir};
      yy = 0;
      break;
    case Left:
      if (0 < x)         return {s, x - 1, y    , dir};
      xx = WIDTH - 1;
      break;
    case Up:
      if (0 < y)         return {s, x    , y - 1, dir};
      yy = WIDTH - 1;
      break;
    default: assert(false);
    }
    auto [side, rotations] = edges.at(s).at(dir);
    side = side - 1;

    auto [xr, yr, d] = rotate(xx, yy, dir, rotations);

    return {side, xr, yr, d};
}

  void step(uint times) {
    for (uint i = 0; i < times; ++i) {
      auto [side, xx, yy, d] = next_tile();
      if (m.at(side).at(yy).at(xx) == '#') {
        return;
      }
      s = side;
      x = xx;
      y = yy;
      dir = d;
    }
  }

  void turn(char turn) {
    assert(turn == 'L' || turn == 'R');
    switch (dir) {
    case Right:
      dir = (turn == 'L') ? Up    : Down ; break;
    case Down:
      dir = (turn == 'L') ? Right : Left ; break;
    case Left:
      dir = (turn == 'L') ? Down  : Up   ; break;
    case Up:
      dir = (turn == 'L') ? Left  : Right; break;
    default: assert(false);
    }
  }

  void walk(std::string& directions) {
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
    std::cout << "SIDE: " << s <<  std::endl;
    for (uint j = 0; j < WIDTH; ++j) {
      for (uint i = 0; i < WIDTH; ++i) {
        if (i == x && j == y) {
          std::cout << "X";
        } else {
          std::cout << m[s][j][i];
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

  std::array<Side, 6> cube;
  for (uint i = 0; i < 6; ++i) {
    cube[i] = side_from_map(map, pos_side[i].first, pos_side[i].second);
  }
  return {cube, line};
}



int main() {
  auto [m, dirs] = read();

  auto p = Player(m);

  p.walk(dirs);

  auto res = 1000 * (pos_side[p.s].second + p.y + 1) + 4 * (pos_side[p.s].first + p.x + 1) + p.dir;

  std::cout << "Result: " << res << std::endl;
  return 0;
}
