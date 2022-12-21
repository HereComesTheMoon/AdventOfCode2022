#include <fstream>
#include <iostream>
#include <vector>
#include <stdint.h>    
#include <map>
#include <assert.h>
#include <optional>
#include <limits>

const std::string INPUT_FILE = "./data/21.txt";

typedef std::pair<std::string, std::string> Monkey;

bool is_number(std::string s) {
  for (const auto& c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

std::vector<Monkey> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<Monkey>();

  while (std::getline(infile, line)) {
    std::string name = line.substr(0, 4);
    line.erase(0, 6);
    data.emplace_back(name, line);
  }

  return data;
}

std::optional<int64_t> get_val(std::string name, std::string s, const std::map<std::string, int64_t>& known) {
  if (known.contains(name)) { return known.at(name); }
  if (is_number(s)) { return std::stoi(s); }
  char op = s.at(5);
  auto sa = s.substr(0, 4);
  auto sb = s.substr(7, 4);
  if (known.contains(sa) && known.contains(sb)) {
    auto a = known.at(sa);
    auto b = known.at(sb);
    switch (op) {
      case '+':
        return a + b;
        break;
      case '*':
        if (a > std::numeric_limits<int64_t>::max() / b) { assert(false); }
        return a * b;
        break;
      case '/':
        assert(a == (a/b)*b);
        return a / b;
        break;
      case '-':
        return a - b;
        break;
      default:
        assert(false);
    }
  }
  return std::nullopt;
}

int64_t compute(const std::vector<Monkey>& v) {
  auto known = std::map<std::string, int64_t>();
  
  while (!known.contains("root")) {
    for (const auto& [name, s] : v) {
      if (known.contains(name)) { continue; }
      auto value = get_val(name, s, known);
      if (value.has_value()) { known.insert({name, value.value()}); }
    }
  }

  return known.at("root");
} 


int main() {
  auto data = read();
  auto res = compute(data);
  std::cout << "Result: " << res << std::endl;
  return 0;
}
