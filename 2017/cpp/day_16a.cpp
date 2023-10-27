#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cmath>
#include <queue> 
#include <numeric> 

std::vector<std::string> parse_input(const std::string& line) {
  std::vector<std::string> instructions;
  std::size_t start = 0;
  const std::string delimiter = ",";
  std::size_t end = line.find(delimiter);
  while(end != std::string::npos) {
    instructions.emplace_back(line.substr(start, end - start));
    start = end + delimiter.size();
    end = line.find(delimiter, start);
  }
  instructions.emplace_back(line.substr(start, end - start));
  return instructions;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_16_input" ;  
  constexpr int n = 16;
  std::ifstream file(input);
  std::string line;
  std::array<int, n> programs;
  std::iota(std::begin(programs), std::end(programs), 0);

  std::getline(file, line);
  const std::vector<std::string> instructions = parse_input(line);

  for(const auto& instruction : instructions) {
    // std::cout << '|' << instruction << '|' << '\n';
    if (instruction[0] == 's') {
      const std::size_t number = std::stoull(instruction.substr(1, instruction.size() - 1)) % 16;
      std::rotate(std::begin(programs), std::next(std::begin(programs), programs.size() - number), std::end(programs));
    }  else if (instruction[0] == 'x') {
      const auto idx = instruction.find('/');
      const auto position_1 = std::stoull(instruction.substr(1, idx - 1));
      const auto position_2 = std::stoull(instruction.substr(idx + 1, instruction.size() - idx - 1));
      std::swap(programs[position_1], programs[position_2]);
    } else if (instruction[0] == 'p') {
      auto it_1 = std::find(std::begin(programs), std::end(programs), instruction[1]-'a');
      auto it_2 = std::find(std::begin(programs), std::end(programs), instruction[3]-'a');
      std::iter_swap(it_1, it_2);
    }
  } 
  for (const auto program:programs) {
    std::cout << char('a' + program);
  }
  std::cout << '\n';
  return 0;
}