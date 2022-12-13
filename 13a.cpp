#include <deque>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <string_view>
#include <string>
#include <variant>
#include <memory>
#include <compare>

const std::string INPUT_FILE = "./data/13.txt";

typedef std::pair<std::string, std::string> pair;

class Packet;

typedef std::variant<int, std::unique_ptr<Packet>> val;

size_t get_arg(std::string_view s) {
  assert(s.size() != 0);
  int count = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    // std::cout << s[i];
    // std::cout.flush();
    assert (count >= 0);
    switch (s[i]) {
      case '[': ++count; break;
      case ']': --count; break;
      case ',': if (count == 0) return i; 
      default: ;
    }
  }

  // assert(false);
  return s.size();
}

class Packet {
public:
  std::vector<val> content;

  Packet(std::string_view s) {
    // These asserts were just for tests. They should fail in some cases
    // assert (s.size() != 0);
    // assert (s[0] != '[');
    // assert (s.back() != ']');
    content = std::vector<val>();

    std::string word;
    
    size_t i = 0;
    while (i < s.size()) {
      auto j = get_arg(s.substr(i, s.size()));
      auto t = s.substr(i, j);
      // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
      // assert("1" == t);
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
    // for (auto &value: content) {
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
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << "This:" << std::endl;
    // (*this).print();
    // std::cout << std::endl;
    // other.print();
    for (size_t i = 0; i < std::min(content.size(), other.content.size()); ++i) {
      bool a = std::holds_alternative<int>(content[i]);
      bool b = std::holds_alternative<int>(other.content[i]);
      std::weak_ordering comp = 0 <=> 0;
      if (a && b) {
        comp = std::get<int>(content[i]) <=> std::get<int>(other.content[i]);
      } else if (!a && !b) {
        // std::cout << "bing bing wahoo" << std::endl;
        comp = *std::get<1>(content[i]) <=> *std::get<1>(other.content[i]);
      } else if (a) {
        Packet temp = Packet(std::to_string(std::get<int>(content[i])));
        comp = temp <=> *std::get<1>(other.content[i]);
        // comp = temp <=> other;
      } else {
        Packet temp = Packet(std::to_string(std::get<int>(other.content[i])));
        comp = *std::get<1>(content[i]) <=> temp;
        // comp = *this <=> temp;
      }
      if (comp == 0) continue;
      return comp;
    }
        // std::cout << "bing bing wahoo" << std::endl;
    return content.size() <=> other.content.size();
  }
};

std::vector<std::pair<Packet, Packet>> read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<std::pair<Packet, Packet>> data;
  std::string first;
  std::string second;
  while (std::getline(infile, first)) {
    std::getline(infile, second);
    data.emplace_back(Packet(first.substr(1, first.size() - 2)), Packet(second.substr(1, second.size() - 2)));
    std::getline(infile, first);
  }
  return data;
}

// std::string_view find_sublist() {
  
// }

// int compare(std::string_view l, std::string_view r) {
  
// }

void test1() {
  std::string s = "1,1,3,1,1";
  auto i = 0;
  auto j = get_arg(s.substr(i, s.size()));
  auto t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("3" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);
}


void test2() {
  std::string s = "[1],[2,3,4]";
  auto i = 0;
  auto j = get_arg(s.substr(i, s.size()));
  auto t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("[1]" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("[2,3,4]" == t);
}

void test3() {
  std::string s = "1,[2,[3,[4,[5,6,7]]]],8,9";
  auto i = 0;
  auto j = get_arg(s.substr(i, s.size()));
  auto t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("1" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("[2,[3,[4,[5,6,7]]]]" == t);

  i = i + j + 1;
  j = get_arg(s.substr(i, s.size()));
  t = s.substr(i, j);
  // std::cout << "i: " << i << ", j: " << j << ", t: " << t  << std::endl;
  assert("8" == t);
}
int main() {
  auto input = read();

  int res = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    auto &[v, w] = input[i];
    if (v <= w) {
      res += (i + 1);
    }
  }
  std::cout << "Result: " << res << std::endl;
  // for (auto &[v, w]: input) {
    // std::cout << std::endl;
    // v.print();
    // std::cout << std::endl;
    // w.print();
    // std::cout << std::endl;
    // if (v <= w) {
    //   std::cout << "\nSmaller: ";
    //   v.print();
    // } else {
    //   std::cout << "\nSmaller: ";
    //   w.print();
    // }
    // }
  // }

  return 0;
}
