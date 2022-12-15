#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const std::string INPUT_FILE = "./data/15.txt";
// const int ROW = 10;
const int ROW = 2000000;

// Manhattan distance
uint l1(int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }

class Scanner {
public:
  int sx, sy, bx, by;
  uint r;

  Scanner(std::array<int, 4> input) {
    sx = input[0];
    sy = input[1];
    bx = input[2];
    by = input[3];
    r = l1(sx, sy, bx, by);
  }

  // Range of elements covered by the scanner
  std::pair<int, int> row(int y) const {
    return std::make_pair(
        sx - r + abs(sy - y),
        sx + r - abs(sy - y) + 1 // + 1 for exclusive upper bound
    );
  }
};

std::vector<Scanner> read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<Scanner> input;

  std::string line;
  while (std::getline(infile, line)) {
    std::stringstream iss(line);
    std::string chunk;
    int sx, sy, bx, by;

    std::getline(iss, chunk, '=');
    std::getline(iss, chunk, ',');
    sx = std::stoi(chunk);

    std::getline(iss, chunk, '=');
    std::getline(iss, chunk, ':');
    sy = std::stoi(chunk);

    std::getline(iss, chunk, '=');
    std::getline(iss, chunk, ',');
    bx = std::stoi(chunk);

    std::getline(iss, chunk, '=');
    std::getline(iss, chunk);
    by = std::stoi(chunk);

    input.push_back(Scanner({sx, sy, bx, by}));
  }
  return input;
}

// Given any row, get the overlap of all scanners with this row, sorted
std::vector<std::pair<int, int>> get_intervals(const std::vector<Scanner> scanners, int y) {
  std::vector<std::pair<int, int>> ivs;
  ivs.reserve(scanners.size());
  for (const auto &s : scanners) {
    auto [a, b] = s.row(y);
    if (a < b)
      ivs.emplace_back(a, b);
  }
  std::sort(ivs.begin(), ivs.end());
  return ivs;
}

// Merge all overlapping intervals
void merge(std::vector<std::pair<int, int>> &ivs) {
  int i = 0;
  for (uint j = 1; j < ivs.size(); ++j) {
    auto [a, b] = ivs[j];
    if (a < ivs[i].second) {
      ivs[i].second = std::max(ivs[i].second, b);
    } else {
      ++i;
      ivs[i] = ivs[j];
    }
  }
  ivs.resize(i + 1);
}

int main() {
  auto scanners = read();

  auto ivs = get_intervals(scanners, ROW);

  for (const auto &[a, b] : ivs) {
    std::cout << "[" << a << "," << b << "), ";
  }
  std::cout << std::endl;

  merge(ivs);

  for (const auto &[a, b] : ivs) {
    std::cout << "[" << a << "," << b << "), ";
  }
  std::cout << std::endl;

  auto res = 0;
  for (const auto &[a, b] : ivs) {
    res += b - a;
  }

  std::cout << "Result: " << res - 1 << std::endl;
  return 0;
}
