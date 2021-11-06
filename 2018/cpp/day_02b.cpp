#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

struct Node {
  char val;
  std::unordered_map<char, std::unique_ptr<Node>> m;
};

std::tuple<bool, std::string> find_in_tree(const std::string& s, const int index,
  Node* p, int  jumps_remaining) {
  if (s[index] != p->val && jumps_remaining > 0) {
    if (index == s.size()-1) {
      return {true, ""};
    }
    for (const auto& [c, ptr] : p->m) {
      if (const auto [found, same_chars] = find_in_tree(s, index + 1, ptr.get(), jumps_remaining - 1);
          found) {
        return {found, same_chars};
      }
    }
  } else if (s[index] == p->val) {
    if (index == s.size()-1) {
      return {true, s.substr(index, s.size() - index)};
    }
    for (const auto& [c, ptr] : p->m) {
      if (const auto [found, same_chars] = find_in_tree(s, index + 1, ptr.get(), jumps_remaining);
          found) {
        return {found, s[index] + same_chars};
      }
    }
  }
  return {false, ""};
}

void add_to_tree(const std::string& s, Node* base) {
  auto p = base;
  for(const auto c : s) {
    if(p->m.find(c) == p->m.end()) {
      p->m[c] = std::make_unique<Node>(c);
    }
    p = p->m[c].get();
  }
}

int main(int argc, char* argv[]) {

  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  long long n_2_reps = 0;
  long long n_3_reps = 0;

  std::fstream file(input);
  std::unique_ptr<Node> base = std::make_unique<Node>();
  while (std::getline(file, line)) {
    std::vector<int> counts(26, 0);
    for (const auto c : line) {
      counts[c - 'a']++;
    }

    if(std::find(std::begin(counts), std::end(counts), 2) == std::end(counts) ||
       std::find(std::begin(counts), std::end(counts), 3) == std::end(counts)) {
      if (base->m.find(line[0]) != base->m.end()) {
        if(const auto [in_tree, same_chars] = find_in_tree(line, 0, base->m[line[0]].get(), 1); in_tree) {
          std::cout << same_chars << '\n';
          break;
        }
      }
      add_to_tree(line, base.get());
    }
  }
  return 0;
}
