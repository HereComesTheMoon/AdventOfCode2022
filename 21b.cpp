#include <fstream>
#include <iostream>
#include <vector>
#include <stdint.h>    
#include <map>
#include <assert.h>
#include <limits>

const std::string INPUT_FILE = "./data/21.txt";

typedef std::pair<std::string, std::string> Monkey;

bool is_number(std::string& s) {
  for (const auto& c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

std::map<std::string, std::string> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::map<std::string, std::string>();

  while (std::getline(infile, line)) {
    std::string name = line.substr(0, 4);
    line.erase(0, 6);
    data.insert({name, line});
  }

  data.insert_or_assign("humn", "humn");
  data.at("root")[5] = '=';
  return data;
}

std::string substitute(std::string name, const std::map<std::string, std::string>& dict) {
  auto val = dict.at(name);
  if (is_number(val)) { return val; }
  if (val == "humn") { return val; }

  char op = val.at(5);
  auto sa = val.substr(0, 4);
  auto sb = val.substr(7, 4);
  assert(dict.contains(sa));
  assert(dict.contains(sb));
  auto ta = substitute(sa, dict);
  auto tb = substitute(sb, dict);

  if (is_number(ta) && is_number(tb)) {
    auto a = std::stoll(ta);
    auto b = std::stoll(tb);
    switch (op) {
      case '+':
        return std::to_string(a + b);
      case '*':
        if (a > std::numeric_limits<int64_t>::max() / b) { assert(false); }
        return std::to_string(a * b);
      case '/':
        assert(a == (a/b)*b);
        return std::to_string(a / b);
      case '-':
        return std::to_string(a - b);
      default:
        assert(false);
    }
    
  }

  return "(" + ta + " " + op + " " + tb + ")";
}

std::tuple<std::string, char, std::string> split(std::string s) {
  auto count = 0;
  for (uint i = 1; i < s.size() - 1; ++i) {
    if (count == 0) {
      if (s[i] == '+' || s[i] == '*' || s[i] == '/' || s[i] == '-' || s[i] == '=') {
        return {s.substr(1, i - 2), s[i], s.substr(i + 2, s.size() - i - 3)};
      }
    }
    if (s[i] == '(') count++;
    if (s[i] == ')') count--;
  }

  assert(false);
}

int64_t simplify(std::string root, int64_t equals) {
  if (root == "humn") return equals; 
  auto [a, op, b] = split(root);
  if (is_number(a)) {
    auto aa = std::stoll(a);
    switch (op) {
      case '+': return simplify(b, equals - aa);
      case '*': return simplify(b, equals / aa);
      case '/': assert(false);
      case '-': return simplify(b, aa - equals);
      default :  assert(false);
    }
  } else if (is_number(b)) {
    auto bb = std::stoll(b);
    switch (op) {
      case '+': return simplify(a, equals - bb);
      case '*': return simplify(a, equals / bb);
      case '/': return simplify(a, equals * bb);
      case '-': return simplify(a, equals + bb);
      default : assert(false);
    }
  }

  assert(false);
}


int main() {
  auto data = read();

  auto subs = substitute("root", data);
  auto [a, op, b] = split(subs);
  assert(op == '=');
  int64_t res = 0;

  if (is_number(a)) {
    res = simplify(b, std::stoll(a));
  } else if (is_number(b)) {
    res = simplify(a, std::stoll(b));
  } else { assert(false); }

  std::cout << "Result: " << res << std::endl;
  return 0;
}
