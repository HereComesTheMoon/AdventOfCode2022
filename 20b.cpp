#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>    

const std::string INPUT_FILE = "./data/20.txt";
const int64_t KEY = 811589153;

class Node {
public:
  int64_t val;
  Node* prev;
  Node* next;

  void print() {
    printf("Node: { val: %ld, prev: %ld, next: %ld }\n", val, prev->val, next->val);
  }
};

class Head {
public:
  Node* head;
  std::vector<Node*> nodes;

  void print() {
    Node* n = head;
    do {
      std::cout << n->val << ", ";
      n = n->next;
    } while (n != head);
    std::cout << std::endl;
  }

  Node* walk(Node* node, int64_t steps) {
    int64_t mod = nodes.size() - 1;
    steps = ((steps % mod) + mod) % mod;
    while (steps < 0) {
      node = node->prev;
      ++steps;
    }
    while (steps > 0) {
      node = node->next;
      --steps;
    }
    return node;
  }

  void mix_step(uint num) {
    Node* n = nodes.at(num);
    Node* other = walk(n, n->val);

    if (other == n) {
      return;
    }

    Node* a = n->prev;
    Node* b = n->next;
    a->next = b;
    b->prev = a;

    Node* succ = other->next;
    succ->prev = n;
    other->next = n;
    n->prev = other;
    n->next = succ;
  }
};

Head read_list() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  std::getline(infile, line);
  Node* head = new Node({std::stoi(line) * KEY, nullptr, nullptr});

  auto nodes = std::vector<Node*>();
  nodes.push_back(head);
  Node* prev = head;
  while (std::getline(infile, line)) {
    int64_t val = std::stoi(line);
    Node* n = new Node({
      val * KEY,
      prev,
      nullptr
    });
    prev->next = n;
    prev = n;
    nodes.push_back(n);
  }
  head->prev = prev;
  prev->next = head;

  return Head({head, nodes});
}


int main() {
  auto head = read_list();

  head.head->print();
  for (int i = 0; i < 10; ++i) {
    for (uint x = 0; x < head.nodes.size(); ++x) {
      head.mix_step(x);
    }
  }

  head.print();

  Node* zero = head.head;
  while (zero->val != 0) {
    zero = zero->next;
  }

  int64_t a = head.walk(zero, 1000)->val;
  int64_t b = head.walk(zero, 2000)->val;
  int64_t c = head.walk(zero, 3000)->val;

  int64_t res = a + b + c;
  std::cout << "Result: " << res << std::endl;
  return 0;
}
