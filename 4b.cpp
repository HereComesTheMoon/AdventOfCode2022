#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

std::vector<std::vector<int>> read() {
  std::ifstream infile("./data/4.txt");
  std::string line;

  int x1, x2, y1, y2;
  char a, b, c;

  auto data = std::vector<std::vector<int>>(0);
  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    ss >> x1 >> a >> x2 >> b >> y1 >> c >> y2;
    data.push_back(std::vector<int>({x1, x2, y1, y2}));
  }
  return data;
}

inline bool overlap(int x1, int x2, int y1, int y2) {
  if (x1 > x2 || y1 > y2) throw "Ohno"; 

  return !(x2 < y1 || y2 < x1);
}


int main() {
  auto v = read();
  int result = 0;

  for (auto &row: v) {
    if (overlap(row[0], row[1], row[2], row[3])) {
      result++;
    }
  }

  std::cout << result << std::endl;
  return 0;
}
