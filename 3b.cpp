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

int val(char c) {
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

  for (uint k = 0; k < v.size(); k += 3) {
    std::set<char> a = std::set<char>(v[k].begin(), v[k].end());
    std::set<char> b = std::set<char>(v[k+1].begin(), v[k+1].end());
    std::set<char> c = std::set<char>(v[k+2].begin(), v[k+2].end());
    std::vector<char> both;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(both));

    for (auto &item: both) {
      if (c.find(item) != c.end()) {
        result += val(item);
        break;
      }
    }

  }

  std::cout << result << std::endl;
  return 0;
}
