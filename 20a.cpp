#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const std::string INPUT_FILE = "./data/20.txt";

class Node {
public:
  int val;
  Node* prev;
  Node* next;

  void print() {
    int a = (prev == nullptr) ? -999 : prev->val;
    int b = (next == nullptr) ? -999 : next->val;
    printf("Node: { val: %d, prev: %d, next: %d }\n", val, a, b);
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
    } while (n != nodes[0]);
    std::cout << std::endl;
  }

  Node* walk(Node* node, int steps) {
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

  Node* find(int val) {
    Node* node = head;
    while (node->val != val) {
      node = node->next;
    }
    return node;
  }

  void mix_step(uint num) {
    // Node* n = find(val);
    // int val = nodes.at(num)->val;
    Node* n = nodes.at(num);
    int val = n->val;
    // std::cout << "val: " << val << " % " << int(nodes.size() - 1) << std::endl;
    // std::cout << "MOD : " << val % int(nodes.size() - 1) << std::endl;
    int mod = int(nodes.size() - 1);
    val = ((val % mod) + mod) % mod;
    Node* other = walk(n, val);
    if (other == n) {
      std::cout << "Hey" << std::endl;
      return;
    }

    Node* a = n->prev;
    Node* b = n->next;
    a->next = b;
    b->prev = a;
    
    if (val < 0) {
      Node* pred = other->prev;
      pred->next = n;
      other->prev = n;
      n->prev = pred;
      n->next = other;
    } else {
      Node* succ = other->next;
      succ->prev = n;
      other->next = n;
      n->prev = other;
      n->next = succ;
    }
  }
};

std::vector<int> read_instructions() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  auto data = std::vector<int>();
  while (std::getline(infile, line)) {
    data.push_back(std::stoi(line));
  }
  
  return data;
}

Head read_list() {
  std::ifstream infile(INPUT_FILE);
  std::string line;

  std::getline(infile, line);
  Node* head = new Node({std::stoi(line), nullptr, nullptr});

  auto nodes = std::vector<Node*>();
  nodes.push_back(head);
  Node* prev = head;
  while (std::getline(infile, line)) {
    int val = std::stoi(line);
    Node* n = new Node({
      val,
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
  // uint e = 6;
  // std::cout << (-62) % e << std::endl;
  
  auto head = read_list();

  auto instructions = read_instructions();

  head.head->print();
  // for (const auto& x: instructions) {
  for (uint x = 0; x < instructions.size(); ++x) {
    // head.print();
    head.mix_step(x);
  }

  head.print();

  auto zero = head.find(0);

  // auto zero = Head({head.find(0)});
  int a = head.walk(zero, 1000)->val;
  int b = head.walk(zero, 2000)->val;
  int c = head.walk(zero, 3000)->val;


  int res = a + b + c;
  std::cout << "Result: " << res << std::endl;
  return 0;
}
