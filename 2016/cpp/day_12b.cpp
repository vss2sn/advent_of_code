#include <array>
#include <fstream>
#include <functional>  
#include <iostream>
#include <ranges>
#include <string>
#include <vector>  
#include <algorithm>

int sv_to_int (const std::string_view sv) {
  bool negative = (sv[0] == '-') ? true : false;
  int n = 0;
  for (int i = negative ? 1 : 0; i < sv.size(); i++) {
    n = n * 10 + (sv[i] - '0');
  }
  if (negative) {
    n *= -1;
  }
  return n;
}

enum class InstructionType {
    CPY, INC, DEC, JNZ
};

struct Instruction {
  InstructionType type;
  int val = 0;
  int val_2 = 0;

  std::function<void(std::array<int, 4>&, int&)> process; 
};



int main(int argc, char* argv[]) {
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::vector<std::string> parsed_lines;
  std::vector<std::vector<std::string_view>> parsed_instructions;

  std::vector<Instruction> instructions;
  std::array<int, 4> registers {{0,0,0,0}};
  while(std::getline(file, line)) {
    parsed_lines.push_back(line);
    parsed_instructions.emplace_back();
    auto& split_line = parsed_instructions.back();
    std::ranges::copy(parsed_lines.back() 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string_view(rng);}), 
                            std::back_inserter(split_line));
    // for (const auto& ele : split_line) {
    //   std::cout << ele << '\n';
    // }
    Instruction instr;
    if (split_line[0] == "cpy") {
      instr.type = InstructionType::CPY; 
      if (char(split_line[1][0]) >= 'a' && char(split_line[1][0]) <= 'd') {
        const int register_1 = split_line[1][0] - 'a';
        const int register_2 = split_line[2][0] - 'a';
        instr.process = [register_1, register_2](std::array<int, 4>& state, int intr_ptr) {
          state[register_2] = state[register_1];
        };
      } else {
        const int val_1 = sv_to_int(split_line[1]);
        const int register_2 = split_line[2][0] - 'a';
        instr.process = [val_1, register_2](std::array<int, 4>& state, int& instr_ptr) {
          state[register_2] = val_1;
        };
      }
    } else if (split_line[0] == "inc") {
      instr.type = InstructionType::INC;
      const int register_1 = split_line[1][0] - 'a';
      instr.process = [register_1](std::array<int, 4>& state, int& instr_ptr) {
        state[register_1]++;
      };
    } else if (split_line[0] == "dec") {
      instr.type = InstructionType::DEC;
      const int register_1 = split_line[1][0] - 'a';
      instr.process = [register_1](std::array<int, 4>& state, int& instr_ptr) {
        state[register_1]--;
      };
    } else if (split_line[0] == "jnz") {
      instr.type = InstructionType::JNZ;
      if (char(split_line[1][0]) >= 'a' && char(split_line[1][0]) <= 'd') {
        const int register_1 = split_line[1][0] - 'a';
        const int val_2 = sv_to_int(split_line[2]);
        instr.process = [register_1, val_2](std::array<int, 4>& state, int& instr_ptr) {
          if (state[register_1] != 0) {
            instr_ptr += val_2;
          } else {
            instr_ptr++;
          }
        };
      } else {
        const int val_1 = sv_to_int(split_line[1]);
        const int val_2 = sv_to_int(split_line[2]);
        instr.process = [val_1, val_2](std::array<int, 4>& state, int& instr_ptr) {
          if (val_1 != 0) {
            instr_ptr += val_2;
          } else {
            instr_ptr++;
          }
        };
      }
    } else {
      std::cout << "This should not happen" << '\n';
      std::cout << line << '\n';
      exit(0);
    }
    instructions.push_back(instr);
  }

  int instr_ptr = 0;
  registers['c' - 'a'] = 1; 
  while(instr_ptr < instructions.size()) {
    // std::cout << '\n';
    // std::cout << parsed_lines[instr_ptr] << '\n';
    // std::cout << "Ptr: " << instr_ptr << '\n';
    // for (const auto& ele : registers) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << " | " << instr_ptr;
    // std::cout << '\n';
    const auto prev_instr_ptr = instr_ptr;
    instructions[instr_ptr].process(registers, instr_ptr);
    if (instructions[prev_instr_ptr].type != InstructionType::JNZ) {
      // std::cout << "Incrementing" << '\n';
      instr_ptr++;
    }
    // for (const auto& ele : registers) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << " | " << instr_ptr;
    // std::cout << '\n';
  }
  std::cout << registers[0] << '\n';
}