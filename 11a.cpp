#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <utility>
#include <unordered_set>
#include <functional>
#include <deque>
#include <algorithm>

const std::string INPUT_FILE = "./data/11.txt";
const size_t number_monkeys = 7;

class Monkey {
public:
  size_t number_inspects;
  Monkey(std::deque<int> itemst, std::function<int(int)> inspectt, int testt, size_t throw_if_truet, size_t throw_if_falset) {
    items = itemst;
    inspect = inspectt;
    test = testt;
    throw_if_true = throw_if_truet;
    throw_if_false = throw_if_falset;
    number_inspects = 0;
  }

  void print() {
    std::cout << "Items: [";
    for (auto &x: items) {
      std::cout << x << ", ";
    }
    std::cout << "]" << std::endl;
  }

  std::pair<size_t, int> toss() {
    if (items.empty()) {
      std::cout << "Empty!" << std::endl;
    }
    int a = items.front();
    items.pop_front();

    a = inspect(a) / 3;
    ++number_inspects;
    if (a % test == 0) {
      return {throw_if_true, a};
    }
    return {throw_if_false, a};
  }

  bool empty() {
    return items.empty();
  }

  void push_back(int a) {
    items.push_back(a);
  }

private:
  std::deque<int> items = std::deque<int>(0);
  std::function<int(int)> inspect;
  int test;
  size_t throw_if_true;
  size_t throw_if_false;
};

class Monkeys {
public: 
  Monkeys(std::vector<Monkey> monkeyst) {
    monkeys = monkeyst;
  }

  void simulate() {
    for (auto &m: monkeys) {
      while (!m.empty()) {
        auto [pos, val] = m.toss();
        monkeys[pos].push_back(val);
      }
    }
  }

  int monkey_business() {
    auto vals = std::vector<int>();
    for (auto &m: monkeys) {
      vals.push_back(m.number_inspects);
    }
    std::cout << "Number inspects:" << std::endl;
    for (auto &num: vals) {
      std:: cout << num << std::endl;
    }

    std::sort(vals.begin(), vals.end());

    std::cout << "Monkey business:" << std::endl;
    int business = vals.back() * vals[vals.size() - 2];
    std::cout << business << std::endl;

    return business;
  }

private:
  std::vector<Monkey> monkeys;
};


void match(std::string pattern, std::string &parse) {
  // std::cout << "Expected: " << pattern << std::endl;
  // std::cout << "Got     : " << parse << std::endl;
  if (parse.size() < pattern.size()) {
    std::cout << "Expected: " << pattern << std::endl;
    std::cout << "Got     : " << parse << std::endl;
    throw std::exception();
  }
  for (size_t k = 0; k < pattern.size(); ++k) {
    if (pattern[k] != parse[k]) {
      std::cout << "Expected: " << pattern << std::endl;
      std::cout << "Got     : " << parse << std::endl;
      throw std::exception();
    }
  }
  parse.erase(0, pattern.size());
}

Monkey read_monkey(std::ifstream &infile) {
  std::string line;

  std::getline(infile, line); 
  match("Monkey ", line);

  std::getline(infile, line);
  std::deque<int> items;
  match("  Starting items: ", line);
  std::istringstream iss(line);
  std::string entry;
  while (std::getline(iss, entry, ',')) {
    // std::cout << entry << std::endl;
    items.push_back(std::stoi(entry));
  }
  iss.clear();

  // std::cout << line << std::endl;
  std::getline(infile, line);
  // std::cout << line << std::endl;
  std::function<int(int)> fun;
  match("  Operation: new = old ", line);
  char op = line[0];
  // std::cout << line.size() << std::endl;
  line.erase(0, 2);
  if (line[0] == 'o') {
    if (op == '+') {
      fun = [] (int x) -> int { return x + x; };
    } else if (op == '*') {
      fun = [] (int x) -> int { return x * x; };
    } else {
      throw std::exception();
    }
  } else {
    int a = std::stoi(line);
    if (op == '+') {
      fun = [a] (int x) -> int { return x + a; };
    } else if (op == '*') {
      fun = [a] (int x) -> int { return x * a; };
    } else {
      throw std::exception();
    }
  }

  std::getline(infile, line);
  match("  Test: divisible by ", line);
  int test = std::stoi(line);
  

  std::getline(infile, line);
  match("    If true: throw to monkey ", line);
  int throw_if_true = std::stoi(line);

  std::getline(infile, line);
  match("    If false: throw to monkey ", line);
  int throw_if_false = std::stoi(line);

  std::getline(infile, line);
  return Monkey(items, fun, test, throw_if_true, throw_if_false);
}

std::vector<Monkey> read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<Monkey> data;
  for (size_t i = 0; i < number_monkeys + 1; ++i) {
     data.push_back(read_monkey(infile));
    
  }

  return data;
}




int main() {
  auto input = read();

  auto monkeys = Monkeys(input);

  for (int k = 0; k < 20; ++k) {
    monkeys.simulate();
  }

  monkeys.monkey_business();
  return 0;
}
