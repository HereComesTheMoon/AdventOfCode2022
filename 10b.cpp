#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <utility>
#include <string>

const std::string INPUT_FILE = "./data/10.txt";

auto read() -> std::vector<std::pair<bool, int>> {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<std::pair<bool, int>>(0);
  while (std::getline(infile, line)) {
    if (line[0] == 'n') { 
      data.emplace_back(false, 0);
    } else {
      line.erase(0, 4);
      int a = std::stoi(line);
      data.emplace_back(true, a);
    }
  }

  return data;
}


int main() {
  auto input = read();

  int x = 1;
  int cycles = 0;
  bool wait_add = false;

  auto result = std::vector<bool>();

  size_t k = 0;
  while (k < input.size()) {
    if (cycles % 40 == x - 1 || cycles % 40 == x || cycles % 40 == x + 1) {
      result.push_back(true);
    } else {
      result.push_back(false);
    }
    ++cycles;
    if (wait_add) {
      x += input[k].second;
      ++k;
      wait_add = false;
      continue;
    }
    auto [is_add, val] = input[k];
    if (is_add) {
      wait_add = true;
    } else {
      ++k;
    }
  }

  for (size_t k = 1; k < result.size(); ++k) {
    std::cout << ((result[k-1]) ? '#' : '.');
    if (k % 40 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << '.' << std::endl;  
  return 0;
}
