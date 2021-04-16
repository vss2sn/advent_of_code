#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void recursiveOrbitCount(
    const std::string& current_object,
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        is_orbitted_by,
    std::unordered_map<std::string, int>& n_orbits) {
  if (is_orbitted_by.find(current_object) == std::end(is_orbitted_by)) {
    n_orbits[current_object] = 0;
  } else {
    n_orbits[current_object] = 0;
    const auto ele_it = is_orbitted_by.find(current_object);
    for (const auto orbitting_object : ele_it->second) {
      recursiveOrbitCount(orbitting_object, is_orbitted_by, n_orbits);
      n_orbits[current_object] += n_orbits[orbitting_object] + 1;
    }
  }
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

  recursiveOrbitCount("COM", is_orbitted_by, n_orbits);
  size_t total = 0;
  for (const auto& [key, val] : n_orbits) {
    total += val;
  }
  std::cout << total << '\n';
  return total;
}
