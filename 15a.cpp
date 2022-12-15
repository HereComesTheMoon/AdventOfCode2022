#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>

const std::string INPUT_FILE = "./data/15.txt";

uint l1(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

// Not used
uint area(uint r) {
  return 2*r*(r+1)+1;
}

// This entire class was kind of unnecessary
// Size of discrete circle with l1-norm: 
// 1, 5, 13, 25
// 1
// 1 + 3 + 1
// 1 + 3 + 5 + 3 + 1
// 1 + 3 + 5 + 7 + 5 + 3 + 1
// diff: 4n
// a_{n+1} = a_n + 4*(n+1)
// Formula: a_n = 2*n*(n+1)+1
class Scanner {
public:
  int sx, sy, bx, by;
  uint r;
  uint a;

  Scanner(std::array<int, 4> input) {
    sx = input[0];
    sy = input[1];
    bx = input[2];
    by = input[3];
    r  = l1(sx, sy, bx, by);
  }

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

std::vector<std::pair<int, int>> get_intervals(const std::vector<Scanner> scanners, int y) {
  std::vector<std::pair<int, int>> ivs;
  ivs.reserve(scanners.size());
  for (const auto &s : scanners) {
    auto [a,b] = s.row(y);
    if (a < b)
      ivs.emplace_back(a, b);
  }
  std::sort(ivs.begin(), ivs.end());
  return ivs;
}

void merge(std::vector<std::pair<int, int>> &ivs) {
  int i = 0;
  for (uint j = 1; j < ivs.size(); ++j) {
    auto [a,b] = ivs[j];
    if (a < ivs[i].second) {
      ivs[i].second = std::max(ivs[i].second, b);
    } else {
      ++i;
      ivs[i] = ivs[j];
    }
  }
  ivs.resize(i+1);
}

int main() {
  auto scanners = read();

  // const int y = 10;
  const int y = 2000000;
  auto ivs = get_intervals(scanners, y);

  for (const auto &[a,b] : ivs) {
    std::cout << "[" << a << "," << b << "), ";
  }
  std::cout << std::endl;

  merge(ivs);

  for (const auto &[a,b] : ivs) {
    std::cout << "[" << a << "," << b << "), ";
  }
  std::cout << std::endl;

  auto res = 0;
  for (const auto &[a,b] : ivs) {
    res += b - a;
  }

  std::cout << "Result: " << res - 1 << std::endl;
  return 0;
}
