#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::string instructions;
  std::getline(file, instructions);
  std::unordered_map<std::string, std::array<std::string, 2>> map;
  const std::regex mask_pattern(R"(([A-Z]+) = \(([A-Z]+), ([A-Z]+)\))");
  std::smatch mask_match;
  while(std::getline(file, line)) {
    std::regex_search(line, mask_match, mask_pattern);
    map[mask_match[1]] = {{mask_match[2], mask_match[3]}};
  }

  std::string current = "AAA";
  int idx = 0;
  std::size_t n_steps = 0;
  while (current != "ZZZ") {
    if (instructions[idx] == 'L') {
      current = map[current][0];
    } else {
      current = map[current][1];
    }
    idx++;
    n_steps++;
    if (idx == instructions.size()) {
      idx = 0;
    }
  }
  std::cout << n_steps << '\n';
  return 0;
}

