#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <vector>

const std::string INPUT_FILE = "./data/11.txt";

typedef unsigned long long ull;

class Monkey {
public:
  size_t number_inspects;
  ull test;
  Monkey(std::deque<ull> itemst, std::function<ull(ull)> inspectt, ull testt,
         size_t throw_if_truet, size_t throw_if_falset) {
    items = itemst;
    inspect = inspectt;
    test = testt;
    throw_if_true = throw_if_truet;
    throw_if_false = throw_if_falset;
    number_inspects = 0;
  }

  void print() {
    std::cout << "Items: [";
    for (auto &x : items) {
      std::cout << x << ", ";
    }
    std::cout << "]" << std::endl;
  }

  std::pair<size_t, ull> toss(ull mod) {
    if (items.empty()) {
      std::cout << "Empty!" << std::endl;
    }
    ull a = items.front();
    items.pop_front();

    a = inspect(a) % mod;
    ++number_inspects;
    if (a % test == 0) {
      return {throw_if_true, a};
    }
    return {throw_if_false, a};
  }

  bool empty() { return items.empty(); }

  void push_back(ull a) { items.push_back(a); }

private:
  std::deque<ull> items = std::deque<ull>(0);
  std::function<ull(ull)> inspect;
  size_t throw_if_true;
  size_t throw_if_false;
};

class Monkeys {
public:
  Monkeys(std::vector<Monkey> monkeyst, ull modt) {
    monkeys = monkeyst;
    mod = modt;
  }

  void simulate() {
    for (auto &m : monkeys) {
      while (!m.empty()) {
        auto [pos, val] = m.toss(mod);
        monkeys.at(pos).push_back(val);
      }
    }
  }

  ull monkey_business() {
    auto vals = std::vector<ull>();
    for (auto &m : monkeys) {
      vals.push_back(m.number_inspects);
    }
    std::cout << "Number inspects:" << std::endl;
    for (auto &num : vals) {
      std::cout << num << std::endl;
    }

    std::sort(vals.begin(), vals.end());

    std::cout << "Monkey business:" << std::endl;
    ull business = vals.back() * vals[vals.size() - 2];
    std::cout << business << std::endl;

    return business;
  }

private:
  std::vector<Monkey> monkeys;
  ull mod;
};

void match(std::string pattern, std::string &parse) {
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
  std::deque<ull> items;
  match("  Starting items: ", line);
  std::istringstream iss(line);
  std::string entry;
  while (std::getline(iss, entry, ',')) {
    items.push_back(std::stoi(entry));
  }
  iss.clear();

  std::getline(infile, line);
  std::function<ull(ull)> fun;
  match("  Operation: new = old ", line);
  char op = line[0];
  line.erase(0, 2);
  if (line[0] == 'o') {
    if (op == '+') {
      fun = [](ull x) -> ull { return x + x; };
    } else if (op == '*') {
      fun = [](ull x) -> ull { return x * x; };
    } else {
      throw std::exception();
    }
  } else {
    ull a = std::stoll(line);
    if (op == '+') {
      fun = [a](ull x) -> ull { return x + a; };
    } else if (op == '*') {
      fun = [a](ull x) -> ull { return x * a; };
    } else {
      throw std::exception();
    }
  }

  std::getline(infile, line);
  match("  Test: divisible by ", line);
  ull test = std::stoll(line);

  std::getline(infile, line);
  match("    If true: throw to monkey ", line);
  ull throw_if_true = std::stoll(line);

  std::getline(infile, line);
  match("    If false: throw to monkey ", line);
  ull throw_if_false = std::stoll(line);

  std::getline(infile, line);
  return Monkey(items, fun, test, throw_if_true, throw_if_false);
}

std::vector<Monkey> read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<Monkey> data;
  while (infile.rdbuf()->in_avail() != 0) {
    data.push_back(read_monkey(infile));
  }

  return data;
}

int main() {
  auto input = read();

  ull mod = 1;
  for (auto &monkey : input) {
    mod *= monkey.test;
  }
  auto monkeys = Monkeys(input, mod);

  for (int k = 0; k < 10000; ++k) {
    monkeys.simulate();
  }

  monkeys.monkey_business();
  return 0;
}
