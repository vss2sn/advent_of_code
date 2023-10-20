#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_05_input" ;  
  std::ifstream file(input);
  std::string line;
  std::size_t n_steps = 0;
  int pointer = 0;
  std::vector<int> instructions;
  while(std::getline(file, line)) {
    instructions.emplace_back(std::stoi(line));
  }
  while (pointer >= 0 && pointer < instructions.size()) {
    const auto prev_pointer = pointer;
    pointer += instructions[pointer];
    if (instructions[prev_pointer] >= 3) {
        instructions[prev_pointer]--;
    } else {
        instructions[prev_pointer]++;
    }
    n_steps++;
  }
  std::cout << n_steps << '\n';
  return 0;
}