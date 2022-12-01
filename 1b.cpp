#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<uint> read() {
  std::ifstream infile("./data/1.txt");
  std::string line;

  auto data = std::vector<uint>(0);
  uint now = 0;
  while (std::getline(infile, line)) {
    if (line == "") {
      data.push_back(now);
      std::push_heap(data.begin(), data.end());
      now = 0;
    } else {
      int a;
      std::istringstream iss(line);
      iss >> a;
      now += a;
    }
  }
  return data;
}

int main() {
  auto v = read();
  uint val = 0;

  std::pop_heap(v.begin(), v.end());
  val += v.back();
  v.pop_back();

  std::pop_heap(v.begin(), v.end());
  val += v.back();
  v.pop_back();

  std::pop_heap(v.begin(), v.end());
  val += v.back();
  v.pop_back();

  std::cout << val << std::endl;
  return 0;
}
