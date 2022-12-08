#include <fstream>
#include <iostream>
#include <vector>
#include <array>

const std::string INPUT_FILE = "./data/8.txt";

std::vector<std::vector<char>> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<std::vector<char>>(0);
  while (std::getline(infile, line)) {
    data.push_back(std::vector<char>(line.begin(), line.end()));
  }

  return data;
}


int scenic_score(const std::vector<std::vector<char>> &forest, int x, int y) {
  int score = 1;
  const std::array<int, 4> dir_x = {1, -1, 0, 0};
  const std::array<int, 4> dir_y = {0, 0, 1, -1};

  for (int k = 0; k < 4; k++) {
    const char height = forest[y][x];
    size_t i = y;
    size_t j = x;
    int trees_seen = 0;

    while (0 < i && i < forest.size()-1 && 0 < j && j < forest[0].size()-1) {
      i = i + dir_y[k];
      j = j + dir_x[k];
      trees_seen++;
      if (height <= forest[i][j]) {
        break;
      }
    }
    score *= trees_seen;
  }
  return score;
}


int main() {
  auto forest = read();

  int result = 0;
  for (size_t y = 0; y < forest.size(); y++) {
    for (size_t x = 0; x < forest[0].size(); x++) {
      result = std::max(result, scenic_score(forest, x, y));
    }
  }

  std::cout << "RESULT:" << std::endl;
  std::cout << result << std::endl;

  return 0;
}
