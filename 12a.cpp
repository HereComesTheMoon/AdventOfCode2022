#include <deque>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>

const std::string INPUT_FILE = "./data/12.txt";

typedef std::pair<size_t, size_t> xy;

struct pair_hash {
  inline std::size_t operator()(const std::pair<int, int> & v) const {
    return 31*v.first + v.second;
  }
};

class Grid {
public:
  std::vector<std::string> grid;
  xy start;
  char goal;
  std::vector<std::vector<int>> dist;
  std::deque<xy> next;

  Grid(std::vector<std::string> gridt) {
    assert(gridt.size() != 0);
    for (auto &row: gridt) {
      assert(row.size() == gridt[0].size());
    }
    grid = gridt;

    dist = std::vector<std::vector<int>>(grid.size());
    for (size_t k = 0; k < grid.size(); ++k) {
      dist.at(k) = std::vector<int>(grid[0].size());
      std::fill(dist[k].begin(), dist[k].end(), -1);
    }

    for (size_t y = 0; y < grid.size(); ++y) {
      for (size_t x = 0; x < grid[0].size(); ++x) {
        if (grid[y][x] == 'S') {
          start = {x, y};
          dist[y][x] = 0;
          grid[y][x] = 'a';
          goto ENDSEARCH;
        }
      }
    }
    ENDSEARCH:

    goal = 'E';

    next = {start};
  }

  std::vector<xy> get_neighbours(size_t x, size_t y) {
    auto neighbours = std::vector<xy>();
    if (0 < x) {
      neighbours.emplace_back(x - 1, y);
    }
    if (x < grid[0].size() - 1) {
      neighbours.emplace_back(x + 1, y);
      }
    if (0 < y) {
      neighbours.emplace_back(x, y - 1);
    }
    if (y < grid.size() - 1) {
      neighbours.emplace_back(x, y + 1);
    }
    return neighbours;
  }

  size_t bfs() {
    while(!next.empty()) {
      auto [x,y] = next.front();
      next.pop_front();
      // print_grid();

      auto neighbours = get_neighbours(x, y);
      for (auto &[xx, yy]: neighbours) {
        if (dist[yy][xx] != -1 || grid[y][x] + 1 < grid[yy][xx]) {
          continue;
        }
        if (grid[yy][xx] == goal) {
          if ('z' <= grid[y][x] + 1) {
            std::cout << "Found! x: " << xx << ", y: " << yy << ", Dist: " << dist[y][x] + 1 << std::endl;
            return dist[y][x] + 1;
          } else {
            continue;
          }
        }
        dist[yy][xx] = dist[y][x] + 1;
        next.emplace_back(xx, yy);
      }
    }
    assert(false);
  }

  void print_grid() {
    for (auto &row: dist) {
      for (auto &d: row) {
        printf("%4d", d);
      }
      printf("\n");
    }
    printf("\n");
  }
};

std::vector<std::string> read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<std::string> data;
  std::string line;
  while (std::getline(infile, line)) {
    data.push_back(line);
  }
  return data;
}

int main() {
  auto input = read();

  Grid grid = Grid(input);
  grid.bfs();

  return 0;
}
