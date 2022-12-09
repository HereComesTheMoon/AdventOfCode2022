#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <utility>
#include <unordered_set>

const std::string INPUT_FILE = "./data/9.txt";

std::vector<std::pair<char, int>> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<std::pair<char, int>>(0);
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    char a;
    int b;
    std::cout << line << std::endl;
    iss >> a >> b;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    data.push_back(std::make_pair(a, b));
  }

  return data;
}

struct pair_hash {
  inline std::size_t operator()(const std::pair<int, int> & v) const {
    return 31*v.first + v.second;
  }
};

class Rope {
public:
  Rope(std::vector<std::pair<char, int>> in) {
    input = in;
  }
  void step(char dir) {
    std::cout << dir << std::endl;
    switch (dir) {
      case 'U': hy++; break;
      case 'R': hx++; break;
      case 'D': hy--; break;
      case 'L': hx--; break;
      default: throw std::exception();
    }
    if (abs(hx-tx) <= 1 && abs(hy-ty) <= 1) {
    } else if (hx == tx) {
      ty += (hy - ty < 0) ? -1 : 1;
    } else if (hy == ty) {
      tx += (hx - tx < 0) ? -1 : 1;
    } else {
      tx += (hx - tx < 0) ? -1 : 1;
      ty += (hy - ty < 0) ? -1 : 1;
    }
    seen.insert(std::make_pair(tx, ty));
  }

  auto simulate() {
    for (auto p: input) {
      for (int k = 0; k < p.second; ++k) {
        step(p.first);
      }
    }

    for(int y = -64; y < 52; ++y) {
      for(int x = -100; x < 47; ++x) {
        if (y == 0 && x == 0) {
          std::cout << "S";
        } else {
          std::cout << ((seen.find(std::make_pair(x, y)) != seen.end()) ? "X" : "."); 
        }
      }
      std::cout << std::endl;
    }

    return seen.size();
  }

private:
  int hx = 0;
  int hy = 0;
  int tx = 0;
  int ty = 0;
  std::vector<std::pair<char, int>> input;
  std::unordered_set<std::pair<int, int>, pair_hash> seen = std::unordered_set<std::pair<int, int>, pair_hash>();
};

int main() {
  auto input = read();

  // for (auto row: input) {
    // std::cout << row.first << std::endl;
  // }

  Rope r = Rope(input);
  int result = r.simulate();
  
  std::cout << "RESULT:" << std::endl;
  std::cout << result << std::endl;

  return 0;
}
