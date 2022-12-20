#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <unistd.h>

const std::string INPUT_FILE = "./data/20t.txt";

class Node {
public:
  int val;
  uint prev;
  uint next;

  void print() {
    printf("Node: { val: %d, prev: %d, next: %d }\n", val, prev, next);
    // std::cout << "Node: { val: "
  }
};

class File {
public: 
  std::vector<Node> nodes;

  File(std::vector<Node>&& input) : nodes( std::move ( input )) {}

  void print() {
    Node n = nodes[0];
    int start = n.val;
    do {
      std::cout << n.val << ", ";
      // std::cout.flush();
      n = nodes[n.next];
    } while (n.val != start);
    std::cout << std::endl;
  }

  Node walk(Node head, int steps) {
    // if (steps == 0) return head;
    while (steps < 0) {
      head = nodes[head.prev];
      ++steps;
    }
    while (steps > 0) {
      head = nodes[head.next];
      --steps;
    }
    return head;
  }

  Node find(int val) {
    Node head = nodes[0];
    while (head.val != val) {
      head = nodes[head.next];
    }
    return head;
  }

  void mix_step(int val) {
    Node n = find(val);
    Node other = walk(n, val);
    if (val == 0) return;
    if (val < 0) {
      Node pred = nodes[other.prev];
    }
  }
};

File read() {
  std::ifstream infile(INPUT_FILE);

  std::vector<Node> input;

  std::string line;

  std::getline(infile, line);
  input.push_back(Node({std::stoi(line), 0, 1}));

  uint i = 1;
  while (std::getline(infile, line)) {
    int val = std::stoi(line);
    input.push_back(Node({val,i-1,i+1}));
    i++;
  }

  input[0].prev = input.size() - 1;
  input.back().next = 0;
  return input;
}


int main() {
  auto data = read();

  // data.walk();

  int res = 0;
  std::cout << "Result: " << res << std::endl;
  return 0;
}
