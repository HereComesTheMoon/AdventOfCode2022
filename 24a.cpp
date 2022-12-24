#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <array>

typedef std::vector<std::vector<char>> Map;

enum State { 
  NIL = '.',
  N   = 'N',
  S   = 'S',
  W   = 'W',
  E   = 'E',
  ELF = '#'
};

const std::string INPUT_FILE = "./data/23.txt";
const uint PADDING = 55 + 1;
const std::array<std::pair<int, int>, 8> NEIGHBOURS = {{
  { 1,  0},
  {-1,  0},
  { 0,  1},
  { 0, -1},
  { 1,  1},
  { 1, -1},
  {-1,  1},
  {-1, -1},
}};


class Elves {
public:
  Map m;
  uint time = 0;

  Elves(const Map& map) {
    m = map;
  }
  
  void print() const {
    std::cout <<  "THIS IS ROUND: " << time << std::endl;
    for (const auto& row : m) {
      std::cout << std::endl;
      for (const auto& x : row) {
        std::cout << x;
      }
    }
    std::cout << std::endl;
  }

  void propose() {
    for (uint y = 0; y < m.size(); ++y) {
      for (uint x = 0; x < m[0].size(); ++ x) {
        if (m[y][x] == NIL) continue;
        assert(m[y][x] == ELF);
        if (!is_anyone_close(x, y)) continue;

        m[y][x] = dir(x, y);
      }
    }
  }

  void step() {
    time++;
    std::vector<std::pair<uint, uint>> moving;
    for (uint y = 1; y < m.size() - 1; ++y) {
      for (uint x = 1; x < m[0].size() - 1; ++ x) {
        if (m[y][x] != NIL) continue;
        moving.resize(0);
        who_is_moving_here(moving, x, y);
        if (moving.size() == 1) {
          auto [xx, yy] = moving.front();
          m[yy][xx] = NIL;
          m[y][x] = ELF;
        } else {
          for (const auto& [xx, yy] : moving) {
            m[yy][xx] = ELF;
          }
        }
      }
    }
  }

  uint step_until_stable() {
    while (true) {
      propose();
      print();
      if (!does_anyone_want_to_move()) break;
      step();
    }
    ++time;
    return time;
  }

private:
  bool does_anyone_want_to_move() const {
    for (const auto& row : m) {
      for (const auto& el : row) {
        if (el != NIL && el != ELF) return true;
      }
    }
    return false;
  }

  bool is_anyone_close(uint x, uint y) {
    for (const auto& [i, j] : NEIGHBOURS) {
      if (m.at(y + j).at(x + i) != NIL) return true;       
    }
    return false;
  }

  State dir(uint x, uint y) const {
    switch (time % 4) {
      case 0:
        if (m[y-1][x-1] == NIL && m[y-1][x] == NIL && m[y-1][x+1] == NIL) return N;
        if (m[y+1][x-1] == NIL && m[y+1][x] == NIL && m[y+1][x+1] == NIL) return S;
        if (m[y-1][x-1] == NIL && m[y][x-1] == NIL && m[y+1][x-1] == NIL) return W;
        if (m[y-1][x+1] == NIL && m[y][x+1] == NIL && m[y+1][x+1] == NIL) return E;
        return ELF;
      case 1:
        if (m[y+1][x-1] == NIL && m[y+1][x] == NIL && m[y+1][x+1] == NIL) return S;
        if (m[y-1][x-1] == NIL && m[y][x-1] == NIL && m[y+1][x-1] == NIL) return W;
        if (m[y-1][x+1] == NIL && m[y][x+1] == NIL && m[y+1][x+1] == NIL) return E;
        if (m[y-1][x-1] == NIL && m[y-1][x] == NIL && m[y-1][x+1] == NIL) return N;
        return ELF;
      case 2:
        if (m[y-1][x-1] == NIL && m[y][x-1] == NIL && m[y+1][x-1] == NIL) return W;
        if (m[y-1][x+1] == NIL && m[y][x+1] == NIL && m[y+1][x+1] == NIL) return E;
        if (m[y-1][x-1] == NIL && m[y-1][x] == NIL && m[y-1][x+1] == NIL) return N;
        if (m[y+1][x-1] == NIL && m[y+1][x] == NIL && m[y+1][x+1] == NIL) return S;
        return ELF;
      case 3:
        if (m[y-1][x+1] == NIL && m[y][x+1] == NIL && m[y+1][x+1] == NIL) return E;
        if (m[y-1][x-1] == NIL && m[y-1][x] == NIL && m[y-1][x+1] == NIL) return N;
        if (m[y+1][x-1] == NIL && m[y+1][x] == NIL && m[y+1][x+1] == NIL) return S;
        if (m[y-1][x-1] == NIL && m[y][x-1] == NIL && m[y+1][x-1] == NIL) return W;
        return ELF;
    }
    assert(false);
  } 

  void who_is_moving_here(std::vector<std::pair<uint, uint>>& found, uint x, uint y) {
    if (m[y-1][x] == S) found.emplace_back(x, y-1);
    if (m[y+1][x] == N) found.emplace_back(x, y+1);
    if (m[y][x-1] == E) found.emplace_back(x-1, y);
    if (m[y][x+1] == W) found.emplace_back(x+1, y);
  }

};


Map read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;
  std::getline(infile, line);
  const uint width = line.size();

  Map map;
  map.resize(PADDING);
  for (uint i = 0; i < PADDING; ++i) {
    map.at(i).resize(PADDING * 2 + width, NIL);
  }
  do {
    map.push_back({});
    map.back().resize(PADDING, NIL);
    for (const auto& c : line) {
      map.back().emplace_back(c);
    }
    map.back().resize(PADDING * 2 + width, NIL);
  } while (std::getline(infile, line));

  for (uint i = 0; i < PADDING; ++i) {
    map.push_back({});
    map.back().resize(PADDING * 2 + width, NIL);
  }
  return map;
}




int main() {
  auto game = Elves(read());

  game.print();

  auto res = game.step_until_stable();

  std::cout << "Result: " << res << std::endl;
  return 0;
}
