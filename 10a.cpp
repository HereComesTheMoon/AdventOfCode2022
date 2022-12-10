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

  int result = 0;

  size_t k = 0;
  while (k < input.size()) {
    ++cycles;
    if ((cycles - 20) % 40 == 0 and cycles < 221) {
      result += cycles * x;
    }
    if (wait_add) {
      x += input[k].second;
      ++k;
      wait_add = false;
      continue;
    }
    auto [is_add, val] = input[k];
    std::cout << is_add << " " << val << std::endl;
    if (is_add) {
      wait_add = true;
    } else {
      ++k;
    }
  }
  
  std::cout << "RESULT:" << std::endl;
  std::cout << result << std::endl;

  return 0;
}
