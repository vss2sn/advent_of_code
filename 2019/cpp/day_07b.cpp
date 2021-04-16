#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class AmplifierConfigurationProgram {
 public:
  AmplifierConfigurationProgram(const std::vector<int>& program) {
    this->init_program_state = program;
    this->program = program;
  }

  void reset() {
    program = init_program_state;
    complete = false;
    inst_ptr = 0;
  }

  std::vector<int> getNOps(const size_t n, const size_t inst_ptr, int modes) {
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

  int compute(const std::vector<int>& input) {
    int output = -1;
    size_t input_ptr = 0;
    while (program[inst_ptr] != 99) {
      const int instr = program[inst_ptr];
      const int opcode = instr % 100;
      const int modes = instr / 100;
      if (opcode == 1) {
        const auto ops = getNOps(2, inst_ptr, modes);
        program[program[inst_ptr + 3]] = ops[0] + ops[1];
        inst_ptr += 4;
      } else if (opcode == 2) {
        const auto ops = getNOps(2, inst_ptr, modes);
        program[program[inst_ptr + 3]] = ops[0] * ops[1];
        inst_ptr += 4;
      } else if (opcode == 3) {
        program[program[inst_ptr + 1]] = input[input_ptr];
        inst_ptr += 2;
        input_ptr += 1;
      } else if (opcode == 4) {
        output = program[program[inst_ptr + 1]];
        inst_ptr += 2;
        return output;
      } else if (opcode == 5) {
        const auto ops = getNOps(2, inst_ptr, modes);
        if (ops[0] != 0) {
          inst_ptr = ops[1];
        } else {
          inst_ptr += 3;
        }
      } else if (opcode == 6) {
        const auto ops = getNOps(2, inst_ptr, modes);
        if (ops[0] == 0) {
          inst_ptr = ops[1];
        } else {
          inst_ptr += 3;
        }
      } else if (opcode == 7) {
        const auto ops = getNOps(2, inst_ptr, modes);
        if (ops[0] < ops[1]) {
          program[program[inst_ptr + 3]] = 1;
        } else {
          program[program[inst_ptr + 3]] = 0;
        }
        inst_ptr += 4;
      } else if (opcode == 8) {
        const auto ops = getNOps(2, inst_ptr, modes);
        if (ops[0] == ops[1]) {
          program[program[inst_ptr + 3]] = 1;
        } else {
          program[program[inst_ptr + 3]] = 0;
        }
        inst_ptr += 4;
      }
    }
    complete = true;
    return output;
  }

  bool isComplete() { return complete; }

 private:
  std::vector<int> program;
  std::vector<int> init_program_state;
  bool complete = false;
  size_t inst_ptr = 0;
};

class IntcodeComputer {
 public:
  void addAmplifierConfigurationProgram(const std::vector<int>& program) {
    programs.emplace_back(program);
  }

  int runAmplifierConfigurationProgram(const size_t index,
                                       const std::vector<int>& input) {
    return programs[index].compute(input);
  }

  void resetAmplifierConfigurationProgram(const size_t index) {
    programs[index].reset();
  }

  bool isLastComplete() { return programs.back().isComplete(); }

  bool isComplete(size_t index) { return programs[index].isComplete(); }

  void reset() { programs.clear(); }

 private:
  std::vector<AmplifierConfigurationProgram> programs;
};

int TestAmplifierConfigurationWithFeedbackAndReset(
    IntcodeComputer& computer, const std::vector<int>& program,
    const std::vector<int>& phase_setting) {
  int output = 0;
  int last_valid = -1;
  for (size_t i = 0; i < phase_setting.size(); i++) {
    computer.addAmplifierConfigurationProgram(program);
  }
  for (size_t i = 0; i < phase_setting.size(); i++) {
    output = computer.runAmplifierConfigurationProgram(
        i, {phase_setting[i], output});
  }
  while (!computer.isLastComplete()) {
    for (size_t j = 0; j < phase_setting.size(); j++) {
      output = computer.runAmplifierConfigurationProgram(j, {output});
      if (output != -1) {
        last_valid = output;
      }
    }
  }
  computer.reset();
  return last_valid;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
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

  IntcodeComputer computer;
  int max_output = 0;
  std::vector<int> phase_setting{5, 6, 7, 8, 9};
  do {
    const size_t output = TestAmplifierConfigurationWithFeedbackAndReset(
        computer, program, phase_setting);
    if (output > max_output) {
      max_output = output;
    }
  } while (std::next_permutation(std::begin(phase_setting),
                                 std::end(phase_setting)));
  std::cout << max_output << '\n';
  return max_output;
}
