#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <ranges>

const std::string INPUT_FILE = "./data/14.txt";

typedef std::vector<std::vector<bool>> Grid;
typedef std::vector<std::vector<std::tuple<int, int>>> Input;

class Cave {
public:
  Grid grid;
  int max_x;
  int min_x;
  int max_y;
  int min_y;
  size_t grains = 0;

  Cave(const Input &input) {
    max_x = 500;
    min_x = 500;
    max_y = 0;
    min_y = 0;
    for (auto &structure: input) {
      for (auto &[a, b]: structure) {
        max_x = std::max(max_x, a);
        min_x = std::min(min_x, a);
        max_y = std::max(max_y, b);
        min_y = std::min(min_y, b);
      }
    }
    assert(min_y == 0);

    grid = Grid(max_y - min_y + 1, std::vector<bool>(max_x - min_x + 1));

    for (auto &structure: input) {
      auto [xx, yy] = structure.front();
      for (const auto &[a, b] : structure | std::views::drop(1)) {
        if (xx == a) {
          for (int i = std::min(yy, b); i <= std::max(yy, b); ++i) {
            grid[i - min_y][a - min_x] = true;
          }
        } else {
          assert (yy == b);
          for (int i = std::min(xx, a); i <= std::max(xx, a); ++i) {
            grid[b - min_y][i - min_x] = true;
          }
        }
        xx = a;
        yy = b;
      }
    }
  }

  bool drop() {
    size_t x = 500 - min_x;
    size_t y = 0;

    while(0 <= x && x < grid[0].size() && y < grid.size()) {
      // std::cout << "x : " << x << " , y : " << y << std::endl;
      if (y == grid.size() - 1 || !grid[y+1][x]) {
        ++y;
        continue;
      }
      if (x == 0 || !grid[y+1][x-1]) {
        ++y;
        --x;
        continue;
      }
      if (x == grid[0].size() - 1 || !grid[y+1][x+1]) {
        ++y;
        ++x;
        continue;
      }
      grid[y][x] = true;
      return true;
    }
    return false;
  }

  size_t simulate() {
    while (drop()) ++grains;
    return grains;
  }

  void print() const {
    for (const auto &row: grid) {
      std::cout << std::endl;
      for (const auto &val: row) {
        std::cout << ((val) ? "#" : ".");
      }
    }
    std::cout << std::endl;
  }
};

Input read() {
  std::ifstream infile(INPUT_FILE);

  Input input;

  std::string line;
  while (std::getline(infile, line)) {
    std::stringstream iss(line);
    std::string chunk;
    input.push_back({});
    while (std::getline(iss, chunk, ' ')) {
      std::stringstream chunkss(chunk);
      int a, b;
      char _;
      chunkss >> a >> _ >> b;
      // std::cout << a << " " << b << std::endl;
      input.back().emplace_back(a, b);
      std::getline(iss, chunk, ' ');
    }
  }
  return input;
}



int main() {
  const Input input = read();
  Cave cave(input);
  std::cout << cave.max_x << " " << cave.min_x << std::endl;
  std::cout << cave.max_y << " " << cave.min_y << std::endl;
  cave.print();
  auto res = cave.simulate();
  cave.print();
  std::cout << "Result: " << res << std::endl;
  return 0;
}
