#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  // Get input
  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_str;
  const std::string delimiter = ",";
  std::getline(file, input_str);
  std::size_t start = 0;
  std::size_t end = input_str.find(delimiter);
  std::vector<int> program_main_copy;
  while (end != std::string::npos) {
    program_main_copy.emplace_back(
        std::stoi(input_str.substr(start, end - start)));
    start = end + delimiter.size();
    end = input_str.find(delimiter, start);
  }
  program_main_copy.emplace_back(
      std::stoi(input_str.substr(start, end - start)));

  long long desired_output = 19690720;
  std::vector<int> nv_list(100);
  std::iota(std::begin(nv_list), std::end(nv_list), 0);

  // Solve
  long long ans = -1;
  for (const auto noun : nv_list) {
    for (const auto verb : nv_list) {
      if (noun == verb) continue;
      auto program = program_main_copy;
      program[1] = noun;
      program[2] = verb;
      size_t index = 0;
      while (program[index] != 99) {
        if (program[index] == 1) {
          program[program[index + 3]] =
              program[program[index + 1]] + program[program[index + 2]];
        } else if (program[index] == 2) {
          program[program[index + 3]] =
              program[program[index + 1]] * program[program[index + 2]];
        } else {
          std::cout << "Error" << '\n';
        }
        index += 4;
      }
      if (program[0] == desired_output) {
        ans = noun * 100 + verb;
        std::cout << ans << '\n';
        return ans;
      }
    }
  }
  std::cout << ans << '\n';

  return ans;
}
