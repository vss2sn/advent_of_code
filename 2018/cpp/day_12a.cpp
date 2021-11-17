#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <memory>
#include <numeric>
#include <cassert>
#include <regex>

struct Rule {
  std::string pattern;
  char result;

  Rule (const std::string& pattern, const char result) :
    pattern(pattern), result(result) {}

};

struct Node {
  char c;
  std::unordered_map<char, std::unique_ptr<Node>> next;

  Node (const char c, std::unordered_map<char, std::unique_ptr<Node>> next) : c(c), next(std::move(next)) {}
};

std::unique_ptr<Node> constructTree(const std::vector<Rule>& rules) {
  std::unique_ptr<Node> root = std::make_unique<Node>('r', std::unordered_map<char, std::unique_ptr<Node>>());
  for (const auto& rule : rules) {
    if (rule.result == '.') {
      continue;
    }
    auto node = root.get();
    for (const auto c : rule.pattern) {
      if (node->next.find(c) == node->next.end()) {
        node->next[c] = std::make_unique<Node>(c, std::unordered_map<char, std::unique_ptr<Node>>());
      }
      node = node->next[c].get();
    }
  }
  return root;
}
bool inTree(Node* node, const int index, const int remaining_length,
  const std::vector<char>& pots) {
  if (remaining_length == 0 && node->next.empty()) return true;
  // second is a sanity check
  if (node->next.find(pots[index]) == node->next.end()) return false;
  return inTree(node->next[pots[index]].get(), index + 1, remaining_length - 1, pots);
}

void print(const std::vector<char>& v) {
  for (const auto c : v) {
    std::cout << c << ' ';
  }
  std::cout << '\n';
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  std::string initial_state = line.substr(15, line.size() - 15);
  constexpr int n_steps = 20;
  std::vector<char> temp(n_steps, '.');
  std::vector<char> state = temp;
  state.insert(state.end(), initial_state.begin(), initial_state.end());
  state.insert(state.end(), temp.begin(), temp.end());
  auto new_state = state;
  std::getline(file, line);
  std::regex pattern(R"(([.#]+) => ([.#]))");
  std::vector<Rule> rules;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    // const char result = std::string(match[2])[0];
    // if (result == '#') {
    rules.emplace_back(match[1], std::string(match[2])[0]);
    // }
    // std::cout << match[1] <<  ' ' << std::string(match[2])[0] << '\n';
  }

  const auto root = constructTree(rules);
  // std::cout << inTree(root.get(), 0, 5, {'.', '.', '.', '#', '#'}) << '\n';
  // std::cout << inTree(root.get(), 0, 5, {'#', '#', '#', '#', '#'}) << '\n';
  // print(state);
  for (int step = 0; step < n_steps; step++) {
    for (int index = 0; index < state.size(); index++) {
      new_state[index+2] = inTree(root.get(), index, 5, state) ? '#' : '.';
    }
    std::swap(state, new_state);
    // print(state);
  }
  // print(state);
  long long sum = 0;
  for (int i = 0; i < state.size(); i++) {
    if (state[i] == '#') {
      sum += (i - n_steps);
    }
  }
  std::cout << sum << '\n';
  return sum;
}
