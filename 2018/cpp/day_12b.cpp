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

struct Rule {
  std::string pattern;
  char result;

  Rule (std::string pattern, const char result) :
    pattern(pattern), result(result) {}
};

struct Node {
  bool c;
  std::unordered_map<bool, std::unique_ptr<Node>> next;
  Node (const char c, std::unordered_map<bool, std::unique_ptr<Node>> next) : c(c), next(std::move(next)) {}
};

struct deque_hash{
  std::size_t operator()(const std::deque<bool>& deq) const {
    return std::accumulate(std::begin(deq), std::end(deq), 0, [](const size_t sum, const bool ele) { return  sum * 10 + ele; } );
  }
};

std::unique_ptr<Node> constructTree(const std::vector<Rule>& rules) {
  std::unique_ptr<Node> root = std::make_unique<Node>(true, std::unordered_map<bool, std::unique_ptr<Node>>());
  for (const auto& rule : rules) {
    if (rule.result == '.') {
      continue;
    }
    auto node = root.get();
    for (const auto c : rule.pattern) {
      const bool b = (c == '#');
      if (node->next.find(b) == node->next.end()) {
        node->next[b] = std::make_unique<Node>(b, std::unordered_map<bool, std::unique_ptr<Node>>());
      }
      node = node->next[b].get();
    }
  }
  return root;
}

bool inTree(Node* node, const int index, const int remaining_length,
  const std::deque<bool>& pots) {
  if (node->next.find(pots[index]) == node->next.end()) return false;
  else if (remaining_length - 1 == 0) return true;
  else return inTree(node->next[pots[index]].get(), index + 1, remaining_length - 1, pots);
}

void print(const std::deque<bool>& v) {
  for (const auto c : v) {
    std::cout << (c ? '#' : '.');
  }
  std::cout << '\n';
}

inline int addBuffer(std::deque<bool>& new_state) {
  constexpr int count = 4;
  for (int i = 0; i < count; i++) {
    new_state.push_front(false);
    new_state.push_back(false);
  }
  return count;
}

inline int removeBuffer(std::deque<bool>& new_state) {
  int count = 0;
  while (!new_state.empty() && new_state[0] == false) {
    new_state.pop_front();
    count--;
  }
  while (!new_state.empty() && new_state.back() == false) {
    new_state.pop_back();
  }
  return count;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);

  const std::string initial_state = line.substr(15, line.size() - 15);
  constexpr long long n_steps = 50000000000;
  std::deque<bool> state(initial_state.size(), false);
  for (int i = 0; i < initial_state.size(); i++) {
    state[i] = (initial_state[i] == '#');
  }

  std::getline(file, line);
  std::vector<Rule> rules;
  while(std::getline(file, line)) {
    rules.emplace_back(line.substr(0, 5), line[9]);
  }

  const auto root = constructTree(rules);

  long long count = 0;
  // Count is to count the skew in case it repeats every n times,
  // but the entire pattern moves (for eg) i indixes left

  std::unordered_map<std::deque<bool>, std::pair<long long, long long>, deque_hash> seen;
  seen.insert({state, {0, count}});

  long long sum = 0;
  for (long long step = 1; step < n_steps; step++) {
    auto new_state = std::deque<bool>(state.size() + 4, false);
    count += addBuffer(state);
    for (int index = 0; index < state.size(); index++) {
      new_state[index] = inTree(root.get(), index, 5, state);
    }
    count += removeBuffer(new_state) - 2;
    const auto [it, in] = seen.insert({new_state, {step, count}});

    if (!in) {
      const long long first_seen_at = it->second.first;
      const long long count_when_first_seen = it->second.second;
      const long long repeats_every = step - first_seen_at;
      const long long n_repeats = (n_steps - first_seen_at) / repeats_every;
      const long long final_count = it->second.second + (n_repeats)* (count - it->second.second);
      for (long long i = 0; i < new_state.size(); i++) {
        if (new_state[i]) {
          sum += (i - final_count);
        }
      }
      std::cout << sum << '\n';
      break;
    }
    state = new_state;
  }
  return sum;
}
