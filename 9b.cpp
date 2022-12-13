#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <utility>
#include <unordered_set>

const std::string INPUT_FILE = "./data/9.txt";
const int NUMBER_KNOTS = 10;

std::vector<std::pair<char, int>> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<std::pair<char, int>>(0);
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    char a;
    int b;
    iss >> a >> b;
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
  Rope(std::vector<std::pair<char, int>> &in) {
    input = in;
  }

  void print_rope() {
    const int xxx = 100;
    const int yyy = 100;

    std::cout << "[";
    for (auto &[a, b]: rope) {
      std::cout << "(" << a << "," << b << ")";
    }
    std::cout << std::endl;
    for (int y = -yyy; y < yyy; y++) {
      for (int x = -xxx; x < xxx; x++) {
        bool exists = false;
        for (auto &[a, b]: rope) {
          if (a == x && b == y) {
            exists = true;
          }
        }
        std::cout << ((exists) ? "#" : ".");
      }
      std::cout << std::endl;
    }

    
  }

  auto simulate() {
    for (const auto &p: input) {
      for (int k = 0; k < p.second; ++k) {
        switch (p.first) {
          case 'U': ++rope[0].second; break;
          case 'R': ++rope[0].first; break;
          case 'D': --rope[0].second; break;
          case 'L': --rope[0].first; break;
          default: throw std::exception();
        }
        for (auto i = 0; i < NUMBER_KNOTS - 1; ++i) {
          step(rope[i].first, rope[i].second, rope[i+1].first, rope[i+1].second);
        }
        seen.insert(std::make_pair(rope.back().first, rope.back().second));
      }
      // print_rope();
    }
    return seen.size();
  }

private:
  std::vector<std::pair<int, int>> rope = std::vector<std::pair<int, int>>(10);
  std::vector<std::pair<char, int>> input;
  std::unordered_set<std::pair<int, int>, pair_hash> seen = std::unordered_set<std::pair<int, int>, pair_hash>();

  
  void step(int &hx, int &hy, int &tx, int &ty) {
    if (abs(hx-tx) <= 1 && abs(hy-ty) <= 1) {
    } else if (hx == tx) {
      ty += (hy - ty < 0) ? -1 : 1;
    } else if (hy == ty) {
      tx += (hx - tx < 0) ? -1 : 1;
    } else {
      tx += (hx - tx < 0) ? -1 : 1;
      ty += (hy - ty < 0) ? -1 : 1;
    }
  }
};







int main() {
  auto input = read();

  Rope r = Rope(input);
  int result = r.simulate();
  
  std::cout << "RESULT:" << std::endl;
  std::cout << result << std::endl;

  return 0;
}
