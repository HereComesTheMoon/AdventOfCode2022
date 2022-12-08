#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>

const std::string INPUT_FILE = "./data/8.txt";

std::vector<std::vector<char>> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<std::vector<char>>(0);
  while (std::getline(infile, line)) {
    data.push_back(std::vector<char>(line.begin(), line.end()));
  }

  return data;
}

void check_visible(const std::vector<std::vector<char>> &forest, std::vector<std::vector<bool>> &seen) {
  if (forest.size() != seen.size())
    throw "";
  if (forest[0].size() != seen[0].size())
    throw "";

  for (std::size_t i = 0; i < forest.size(); ++i) {
    int tallest = -1;
    for (std::size_t j = 0; j < forest[0].size(); ++j) {
      if (tallest < forest[i][j]) {
        seen[i][j] = true;
        tallest = forest[i][j];
      }
      if (tallest == 9) {
        break;
      }
    }
  }
}

template <typename T> void rotate(std::vector<std::vector<T>> &m) {
  auto l = m.size();
  auto n = m[0].size();
  for (size_t r = 0; r < m.size() / 2; ++r) {
    for (size_t i = 0; i < m.size() - 1 - 2 * r; ++i) {
      T temp = m[r][r + i];
      m[r][r + i] = m[r + i][n - 1 - r];
      m[r + i][n - 1 - r] = m[l - 1 - r][n - 1 - i - r];
      m[l - 1 - r][n - 1 - i - r] = m[l - 1 - r - i][r];
      m[l - 1 - r - i][r] = temp;
    }
  }
}

int main() {
  auto forest = read();
  auto seen = std::vector<std::vector<bool>>(0);
  for (auto &row : forest) {
    seen.push_back(std::vector<bool>(row.size()));
  }

  for (int i = 0; i < 4; ++i) {
    check_visible(forest, seen);
    rotate(forest);
    rotate(seen);
  }

  // std::cout << "Final configuration:" << std::endl;
  // for (auto &row: forest) {
  //   for (auto &x: row) {
  //     std::cout << x;
  //   }
  //     std::cout << std::endl;
  // }

  int count = 0;
  for (auto &row : seen) {
    for (auto x : row) {
      if (x) {
        count++;
      }
    }
  }

  std::cout << "RESULT:" << std::endl;
  std::cout << count << std::endl;

  return 0;
}
