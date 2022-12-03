#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

std::vector<std::string> read() {
  std::ifstream infile("./data/3.txt");
  std::string line;

  auto data = std::vector<std::string>(0);
  while (std::getline(infile, line)) {
    data.push_back(line);
  }
  return data;
}

inline int val(char c) {
  if ('a' <= c && c <= 'z') {
    return 1 + c - 'a';
  }
  if ('A' <= c && c <= 'Z') {
    return 27 + c - 'A';
  }
  throw std::runtime_error("Unreachable.");
}

int main() {
  auto v = read();
  int result = 0;

  for (auto &row: v) {
    std::set<char> a = std::set<char>(row.begin(), row.begin() + row.length() / 2);
    std::set<char> b = std::set<char>(row.begin() + row.length() / 2, row.end());
    std::vector<char> both;
    both.reserve(1);
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(both));
    if (both.size() != 1) {
      throw "Unreachable!";
    }
    result += val(both[0]);
  }

  std::cout << result << std::endl;
  return 0;
}
