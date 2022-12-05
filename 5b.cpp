#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
#include <tuple>

const uint NUMBER_PILES = 200;
const std::string INPUT_FILE = "./bigboy/5.txt";

std::vector<std::vector<char>> read_crates() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<std::vector<char>>(NUMBER_PILES);
  while (std::getline(infile, line)) {
    if (line[1] == '1') break;
    for (uint k = 0; k < NUMBER_PILES; k++) {
      if (line[1 + k * 4] == ' ') continue;
      data[k].push_back(line[1 + k * 4]);
    }
  }

  for (auto &row: data) {
    std::reverse(row.begin(), row.end());
    for (auto &x: row) {
      std::cout << x << " ";
    }
      std::cout << std::endl;
  }

  return data;
}

std::vector<std::tuple<uint, uint, uint>> read_instructions() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  while (std::getline(infile, line)) {
    if (line.empty()) break;
  }

  auto data = std::vector<std::tuple<uint, uint, uint>>(0);

  std::string token;
  while (std::getline(infile, line)) {
    std::stringstream sline(line);
    auto row_data = std::vector<std::string>(0);
    row_data.reserve(6);

    while (std::getline(sline, token, ' ')) {
      row_data.push_back(token);
    }
      
    data.push_back(std::make_tuple(
        std::stoul(row_data[1]),
        std::stoul(row_data[3]) - 1,
        std::stoul(row_data[5]) - 1
      ));
  }
  return data;
}

void move_crates(std::vector<std::vector<char>> &crates, std::tuple<uint, uint, uint> motion) {
  uint number = std::get<0>(motion);
  auto from = &crates[std::get<1>(motion)];
  auto to = &crates[std::get<2>(motion)];

  to->insert(to->end(), from->end() - number, from->end());
  from->resize(from->size() - number);
}

int main() {
  auto crates = read_crates();
  auto instructions = read_instructions();

  for (auto instruction: instructions) {
    move_crates(crates, instruction);
  }

  std::cout << "Final configuration:" << std::endl;
  for (auto &row: crates) {
    for (auto &x: row) {
      std::cout << x << " ";
    }
      std::cout << std::endl;
  }

  std::cout << "RESULT:" << std::endl;
  for (auto &row: crates) {
    std::cout << row.back();
  }
  std::cout << std::endl;

  return 0;
}
