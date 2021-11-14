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

void complete_letter(const char letter,
  std::unordered_map<char, std::vector<char>>& mapping,
  std::unordered_map<char, std::vector<char>>& inverse_mapping,
  std::vector<char>& letters,
  std::unordered_set<char>& seen,
  std::vector<char>& order){
  if (seen.find(letter) == seen.end()) {
    letters.erase(std::remove(letters.begin(), letters.end(), letter), letters.end());
    seen.insert(letter);
    order.push_back(letter);
    std::cout << "Inserted " << letter << " into order" << '\n';
    for (const auto c : mapping[letter]) {
      letters.emplace_back(c);
      auto& c_inv_map = inverse_mapping[c];
      c_inv_map.erase(std::find(std::begin(c_inv_map), std::end(c_inv_map), letter));
    }
  }
}

std::tuple<bool, char> get_letter(
  std::vector<char>& letters,
  std::unordered_map<char, std::vector<char>>& inverse_mapping,
  std::unordered_set<char>& in_progress
) {
  std:sort(std::begin(letters), std::end(letters));
  const auto it = std::find_if(std::begin(letters), std::end(letters),
    [&inverse_mapping, &in_progress](const char c) {
      return inverse_mapping[c].size() == 0
      && in_progress.find(c) == in_progress.end();
    });
  const bool found = (it != letters.end());
  return {found, found ? *it : '.'};
}

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
  constexpr int n_workers = 5;
  std::vector<int> workers_time(n_workers, 0);
  std::vector<char> workers_letter(n_workers, '.');

  int time_step = 0;
  std::unordered_set<char> in_progress;
  while (!letters.empty()) {
    std::cout << "Time step: " << time_step << '\n';
    for (int i = 0; i < n_workers; i++) {
      if (workers_time[i] <= 0) {
        complete_letter(workers_letter[i], mapping, inverse_mapping, letters, seen, order);
        in_progress.erase(workers_letter[i]);
      }
    }
    for (int i = 0; i < n_workers; i++) {
      if (workers_time[i] <= 0) {
        const auto [letter_available, letter] = get_letter(letters, inverse_mapping, in_progress);
        if (letter_available) {
          workers_time[i] = letter - 'A' + 61;
          workers_letter[i] = letter;
          in_progress.insert(letter);
        }
      }
    }
    for (int i = 0; i < n_workers; i++) {
      workers_time[i]--;
    }
    time_step++;
  }

  time_step--;
  for (const auto c : order) {
    std::cout << c;
  }
  std::cout << '\n';
  std::cout << time_step << '\n';
  return time_step;
}
