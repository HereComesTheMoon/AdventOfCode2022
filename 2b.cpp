#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
// #include <unordered_map> // Doesn't work, pairs can't be hashed. Thanks C++
#include <map>
#include <utility>

const std::map<std::pair<char, char>, uint> round = {
    {std::make_pair('A', 'X'), 0 + 3}, 
    {std::make_pair('A', 'Y'), 3 + 1},
    {std::make_pair('A', 'Z'), 6 + 2},

    {std::make_pair('B', 'X'), 0 + 1}, 
    {std::make_pair('B', 'Y'), 3 + 2},
    {std::make_pair('B', 'Z'), 6 + 3},

    {std::make_pair('C', 'X'), 0 + 2},
    {std::make_pair('C', 'Y'), 3 + 3},
    {std::make_pair('C', 'Z'), 6 + 1},
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
  for (const auto &k : v) {
    result += round.at(k);
  }
  std::cout << result << std::endl;
  return 0;
}
