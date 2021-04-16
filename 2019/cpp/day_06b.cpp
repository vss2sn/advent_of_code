#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::tuple<bool, int> travel(
    const int steps, const std::string& cur, const std::string& dest,
    std::unordered_map<std::string, std::unordered_set<std::string>>&
        is_orbitted_by,
    std::unordered_map<std::string, std::string>& orbits,
    std::unordered_set<std::string>& reached_before) {
  if (cur == dest) {
    return {true, steps};
  }
  if (reached_before.find(cur) != reached_before.end()) {
    return {false, -1};
  }
  reached_before.insert(cur);
  if (is_orbitted_by.find(cur) != is_orbitted_by.end()) {
    for (const auto orbitting_object : is_orbitted_by[cur]) {
      const auto [reached, final_step_count] =
          travel(steps + 1, orbitting_object, dest, is_orbitted_by, orbits,
                 reached_before);
      if (reached) {
        return {reached, final_step_count};
      }
    }
  }
  if (orbits.find(cur) != orbits.end()) {
    const auto [reached, final_step_count] = travel(
        steps + 1, orbits[cur], dest, is_orbitted_by, orbits, reached_before);
    if (reached) {
      return {reached, final_step_count};
    }
  }
  return {false, -1};
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::unordered_map<std::string, std::string> orbits;
  std::unordered_map<std::string, std::unordered_set<std::string>>
      is_orbitted_by;
  std::unordered_map<std::string, int> n_orbits;
  std::unordered_set<std::string> reached_before;

  // Solve
  std::string line;
  const std::string delim = ")";
  while (std::getline(file, line)) {
    const size_t delim_pos = line.find(delim);
    const std::string primary_object = line.substr(0, delim_pos);
    const std::string orbitting_object = line.substr(
        delim_pos + delim.size(), line.size() - delim_pos - delim.size());
    orbits[orbitting_object] = primary_object;
    if (is_orbitted_by.find(primary_object) == std::end(is_orbitted_by)) {
      is_orbitted_by.insert({primary_object, {}});
    }
    is_orbitted_by[primary_object].insert(orbitting_object);
  }

  const auto [reached, n_steps] =
      travel(0, "YOU", "SAN", is_orbitted_by, orbits, reached_before);
  std::cout << n_steps - 2 << '\n';
  return n_steps - 2;
}
