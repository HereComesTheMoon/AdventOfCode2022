#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>


const std::map<std::pair<char, char>, uint> round = {
  {std::make_pair('A', 'X'), 3+1 },
  {std::make_pair('A', 'Y'), 6+2 },
  {std::make_pair('A', 'Z'), 0+3 },

  {std::make_pair('B', 'X'), 0+1 },
  {std::make_pair('B', 'Y'), 3+2 },
  {std::make_pair('B', 'Z'), 6+3 },

  {std::make_pair('C', 'X'), 6+1 },
  {std::make_pair('C', 'Y'), 0+2 },
  {std::make_pair('C', 'Z'), 3+3 },
};

std::vector<std::pair<char, char>> read() {
  std::ifstream infile("./data/2.txt");
  std::string line;

  auto data = std::vector<std::pair<char, char>>(0);
  while (std::getline(infile, line)) {
    char first, second;
    std::istringstream iss(line);
    iss >> first >> second;
    // std::cout << first << " " << second << std::endl;
    data.push_back(std::make_pair(first, second));
  }
  return data;
}

int main() {
  auto v = read();
  uint result = 0;
  for (const auto& k: v) {
    result += round.at(k);
  }
  std::cout << result << std::endl;
  return 0;
}
