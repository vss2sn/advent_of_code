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

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::unordered_map<char, std::vector<char>> mapping;
  std::unordered_map<char, std::vector<char>> inverse_mapping;
  while(std::getline(file, line)) {
    const char map_from = line[5];
    if (line[5] == ' ') {
      continue;
    }
    const char map_to = line[36];
    if (mapping.find(map_from) == mapping.end()) {
      mapping.insert({map_from, {}});
    }
    if (inverse_mapping.find(map_to) == inverse_mapping.end()) {
      inverse_mapping.insert({map_to, {}});
    }
    mapping[map_from].emplace_back(map_to);
    inverse_mapping[map_to].emplace_back(map_from);
  }

  // No prerequisite before these
  std::vector<char> letters;
  for (const auto & [key, val] : mapping) {
    if (inverse_mapping.find(key) == std::end(inverse_mapping)) {
      letters.push_back(key);
    }
  }

  std::unordered_set<char> seen;
  char letter = letters[0];
  std::vector<char> order;
  while (!letters.empty()) {
    if (seen.find(letter) == seen.end()) {
      letters.erase(std::remove(letters.begin(), letters.end(), letter), letters.end());
      order.push_back(letter);
      seen.insert(letter);
      for (const auto c : mapping[letter]) {
        letters.emplace_back(c);
        auto& c_inv_map = inverse_mapping[c];
        c_inv_map.erase(std::find(std::begin(c_inv_map), std::end(c_inv_map), letter));
      }
    }
    std:sort(std::begin(letters), std::end(letters));
    letter = *std::find_if(std::begin(letters), std::end(letters),
      [&inverse_mapping](const char c) {return inverse_mapping[c].size() == 0; });
  }

  for (const auto c : order) {
    std::cout << c;
  }
  std::cout << '\n';
  return 0;
}
