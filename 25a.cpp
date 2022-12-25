#include <algorithm>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

const std::string INPUT_FILE = "./data/25.txt";

std::vector<std::string> read() {
  std::ifstream infile(INPUT_FILE);
  std::string line;
  auto data = std::vector<std::string>(1);
  while (std::getline(infile, data.back())) {
    data.push_back("");
  }

  return data;
}

char digit_to_char(int x) {
  switch (x) {
  case -2: return '=';
  case -1: return '-';
  case 0: return '0';
  case 1: return '1';
  case 2: return '2';
  default: 
      std::cout << x << std::endl;
      assert(false);
  }
}

int64_t char_to_digit(char c) {
  switch (c) {
  case '=': return -2;
  case '-': return -1;
  case '0': return 0;
  case '1': return 1;
  case '2': return 2;
  default: assert(false);
  }
}

int64_t snafu_to_int(const std::string &s) {
  if (s.size() == 0) {
    return 0;
  }
  auto n = s.size();
  int64_t x = 0;
  int64_t b = pow(5, n - 1);
  for (uint i = 0; i < n; ++i) {
    x += char_to_digit(s[i]) * b;
    b /= 5;
  }

  return x;
}

int64_t sum(const std::vector<std::string> &v) {
  int64_t x = 0;
  for (const auto &s : v) {
    x += snafu_to_int(s);
  }
  return x;
}

std::string int_to_snafu(int64_t x) {
  std::string s;
  int64_t b = 5;
  assert(x >= 0);
  while (x != 0) {
    int64_t y = x % b;
    x -= y;
    if (2 < y) {
      y -= 5;
      x += 5;
    }
    x /= 5;
    s.push_back(digit_to_char(y));
  }
  std::reverse(s.begin(), s.end());
  return s;
}

void test() {
  assert(1 == snafu_to_int("1"));
  assert(2 == snafu_to_int("2"));
  assert(3 == snafu_to_int("1="));
  assert(4 == snafu_to_int("1-"));
  assert(5 == snafu_to_int("10"));
  assert(6 == snafu_to_int("11"));
  assert(7 == snafu_to_int("12"));
  assert(8 == snafu_to_int("2="));
  assert(9 == snafu_to_int("2-"));
  assert(10 == snafu_to_int("20"));
  assert(15 == snafu_to_int("1=0"));
  assert(20 == snafu_to_int("1-0"));
  assert(2022 == snafu_to_int("1=11-2"));
  assert(12345 == snafu_to_int("1-0---0"));
  assert(314159265 == snafu_to_int("1121-1110-1=0"));

  assert(int_to_snafu(1) == "1");
  assert(int_to_snafu(2) == "2");
  assert(int_to_snafu(3) == "1=");
  assert(int_to_snafu(4) == "1-");
  assert(int_to_snafu(5) == "10");
  assert(int_to_snafu(6) == "11");
  assert(int_to_snafu(7) == "12");
  assert(int_to_snafu(8) == "2=");
  assert(int_to_snafu(9) == "2-");
  assert(int_to_snafu(10) == "20");
  assert(int_to_snafu(15) == "1=0");
  assert(int_to_snafu(20) == "1-0");
  assert(int_to_snafu(2022) == "1=11-2");
  assert(int_to_snafu(12345) == "1-0---0");
  assert(int_to_snafu(314159265) == "1121-1110-1=0");
}

int main() {
  auto data = read();

  test();

  for (const auto& s : data) {
    auto y = snafu_to_int(s);
    auto ss = int_to_snafu(y);
    assert(ss == s);
  }

  auto x = sum(data);

  auto res = int_to_snafu(x);

  std::cout << snafu_to_int(res) << std::endl;

  std::cout << "Result: " << res << std::endl;
  return 0;
}
