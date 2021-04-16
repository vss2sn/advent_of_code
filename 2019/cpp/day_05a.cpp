#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> getNOps(const size_t n, const size_t inst_ptr, int modes,
                         const std::vector<int>& program) {
  std::vector<int> ops;
  for (size_t i = 1; i <= n; ++i) {
    size_t rem = modes % 10;
    modes = modes / 10;
    if (rem == 0) {
      ops.push_back(program[program[inst_ptr + i]]);
    } else {
      ops.push_back(program[inst_ptr + i]);
    }
  }
  return ops;
}

bool isNumber(const std::string& str) {
  for (char const& c : str) {
    if (std::isdigit(c) == 0) return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_05_input";
  size_t input_position = 1;
  if (argc > 1 && !isNumber(std::string(argv[1]))) {
    input = argv[1];
    input_position += 1;
  }
  std::ifstream file(input);
  std::string input_str;
  std::getline(file, input_str);
  const std::string delimiter = ",";

  std::size_t start = 0;
  std::size_t end = input_str.find(delimiter);
  std::vector<int> program;
  while (end != std::string::npos) {
    program.emplace_back(std::stoi(input_str.substr(start, end - start)));
    start = end + delimiter.size();
    end = input_str.find(delimiter, start);
  }
  program.emplace_back(std::stoi(input_str.substr(start, end - start)));

  // Solve
  size_t inst_ptr = 0;
  int output = -1;
  while (program[inst_ptr] != 99) {
    const int instr = program[inst_ptr];
    const int opcode = instr % 100;
    const int modes = instr / 100;
    if (opcode == 1) {
      const auto ops = getNOps(2, inst_ptr, modes, program);
      program[program[inst_ptr + 3]] = ops[0] + ops[1];
      inst_ptr += 4;
    } else if (opcode == 2) {
      const auto ops = getNOps(2, inst_ptr, modes, program);
      program[program[inst_ptr + 3]] = ops[0] * ops[1];
      inst_ptr += 4;
    } else if (opcode == 3) {
      program[program[inst_ptr + 1]] = std::stoi(argv[input_position]);
      inst_ptr += 2;
      input_position += 1;
    } else if (opcode == 4) {
      output = program[program[inst_ptr + 1]];
      inst_ptr += 2;
    }
  }
  std::cout << output << '\n';
  return output;
}
