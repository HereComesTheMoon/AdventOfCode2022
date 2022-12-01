#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<uint> read() {
  std::ifstream infile("./data/1.txt");
  std::string line;

  auto data = std::vector<uint>(1);
  while (std::getline(infile, line)) {
    if (line == "") {
      data.push_back(0);
    } else {
      int a;
      std::istringstream iss(line);
      iss >> a;
      data.back() += a;
    }
  }
  return data;
}

int main() {
  auto v = read();
  uint max = 0;
  for (auto &x: v) {
    max = std::max(max, x);
  }
  std::cout << max << std::endl;
  return 0;
}
