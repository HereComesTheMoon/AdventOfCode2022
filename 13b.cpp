#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <variant>
#include <memory>
#include <algorithm>

const std::string INPUT_FILE = "./data/13.txt";

class Packet;

typedef std::variant<int, std::unique_ptr<Packet>> val;

size_t get_arg(std::string_view s) {
  assert(s.size() != 0);
  int count = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    assert (count >= 0);
    switch (s[i]) {
      case '[': ++count; break;
      case ']': --count; break;
      case ',': if (count == 0) return i; 
      default: ;
    }
  }

  return s.size();
}

class Packet {
public:
  std::vector<val> content;

  Packet(std::string_view s) {
    content = std::vector<val>();

    std::string word;
    
    size_t i = 0;
    while (i < s.size()) {
      auto j = get_arg(s.substr(i, s.size()));
      auto t = s.substr(i, j);
      if (t.at(0) == '[') {
        val value = std::make_unique<Packet>(Packet(t.substr(1, t.size() - 2)));
        content.push_back(std::move(value));
      } else {
        int a = std::stoi(std::string(t));
        content.push_back(a);
      }

      i = i + j + 1;
    }
  }

  void print() const {
    std::cout << "[";
    for (size_t i = 0; i < content.size(); ++i) { 
      auto value = &content[i];
      if (std::holds_alternative<int>(*value)) {
        std::cout << std::get<int>(*value);
        } else {
        std::get<1>(*value)->print();
      }
      if (i != content.size() - 1) std::cout << ",";
    }
    std::cout << "]";
    std::cout.flush();
  }

  std::weak_ordering operator<=>(const Packet &other) const {
    for (size_t i = 0; i < std::min(content.size(), other.content.size()); ++i) {
      bool a = std::holds_alternative<int>(content[i]);
      bool b = std::holds_alternative<int>(other.content[i]);
      std::weak_ordering comp = 0 <=> 0;
      if (a && b) {
        comp = std::get<int>(content[i]) <=> std::get<int>(other.content[i]);
      } else if (!a && !b) {
        comp = *std::get<1>(content[i]) <=> *std::get<1>(other.content[i]);
      } else if (a) {
        Packet temp = Packet(std::to_string(std::get<int>(content[i])));
        comp = temp <=> *std::get<1>(other.content[i]);
      } else {
        Packet temp = Packet(std::to_string(std::get<int>(other.content[i])));
        comp = *std::get<1>(content[i]) <=> temp;
      }
      if (comp == 0) continue;
      return comp;
    }
    return content.size() <=> other.content.size();
  }
};

std::vector<Packet> read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<Packet> data;
  std::string line;
  while (std::getline(infile, line)) {
    if (line == "") continue;
    data.emplace_back(Packet(line.substr(1, line.size() - 2)));
  }
  return data;
}

void test1() {
  std::string s = "1,1,3,1,1";
  auto i = 0;
  auto j = get_arg(s.substr(i, s.size()));
  auto t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("3" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);
}


void test2() {
  std::string s = "[1],[2,3,4]";
  auto i = 0;
  auto j = get_arg(s.substr(i, s.size()));
  auto t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("[1]" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("[2,3,4]" == t);
}

void test3() {
  std::string s = "1,[2,[3,[4,[5,6,7]]]],8,9";
  auto i = 0;
  auto j = get_arg(s.substr(i, s.size()));
  auto t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("[2,[3,[4,[5,6,7]]]]" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("8" == t);
}
int main() {
  auto input = read();

  Packet key_a = Packet("[2]");
  const Packet key_b = Packet("[6]");

  input.push_back(Packet("[2]"));
  input.push_back(Packet("[6]"));
  
  std::sort(input.begin(), input.end());

  int res = 1;

  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] <=> key_a == 0 || input[i] <=> key_b == 0) 
      res *= (i+1);
  }
  
  std::cout << "Result: " << res << std::endl;
  return 0;
}
