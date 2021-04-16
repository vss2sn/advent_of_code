#include <fstream>
#include <iostream>
#include <iterator>
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
  std::vector<int> program;
  while (end != std::string::npos) {
    program.emplace_back(std::stoi(input_str.substr(start, end - start)));
    start = end + delimiter.size();
    end = input_str.find(delimiter, start);
  }
  program.emplace_back(std::stoi(input_str.substr(start, end - start)));

  // Modify input according to puzzle instructions
  program[1] = 12;
  program[2] = 2;

  // Solve
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
  std::cout << program[0] << '\n';
  return program[0];
}
