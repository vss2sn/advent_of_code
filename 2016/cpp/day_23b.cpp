#include <array>
#include <fstream>
#include <functional>  
#include <iostream>
#include <ranges>
#include <string>
#include <vector>  
#include <algorithm>
#include <variant>
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
    CPY, INC, DEC, JNZ, TGL
};

struct Instruction {
  InstructionType type;
  int val = 0;
  int val_2 = 0;
  bool toggled = false;

  std::function<void(std::array<int, 4>&, int&, std::vector<Instruction>& instrs)> process; 
  std::function<void(std::array<int, 4>&, int&, std::vector<Instruction>& instrs)> process_toggled; 
  void process_wrapper (std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
    if (toggled) {
      process_toggled(state, instr_ptr, instrs);
    } else {
      process(state, instr_ptr,instrs);
    }
  }
};


Instruction createInstruction(const std::vector<std::string>& split_line) {
  std::cout << split_line[0] << '\n';
  Instruction instr;
  if (split_line[0] == "cpy") {
    instr.type = InstructionType::CPY; 
    if (char(split_line[1][0]) >= 'a' && char(split_line[1][0]) <= 'd') {
      const int register_1 = split_line[1][0] - 'a';
      const int register_2 = split_line[2][0] - 'a';
      instr.process = [register_1, register_2](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
        state[register_2] = state[register_1];
        instr_ptr++;
      };
    } else if (char(split_line[2][0]) >= 'a' && char(split_line[2][0]) <= 'd'){
      const int val_1 = sv_to_int(split_line[1]);
      const int register_2 = split_line[2][0] - 'a';
      instr.process = [val_1, register_2](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
        state[register_2] = val_1;
        instr_ptr++;
      };
    } else {
      instr.process = [](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
        instr_ptr++;
        return;
      };
    }
  } else if (split_line[0] == "inc") {
    instr.type = InstructionType::INC;
    const int register_1 = split_line[1][0] - 'a';
    instr.process = [register_1](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
      instr_ptr++;
      state[register_1]++;
    };
  } else if (split_line[0] == "dec") {
    instr.type = InstructionType::DEC;
    const int register_1 = split_line[1][0] - 'a';
    instr.process = [register_1](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
      instr_ptr++;
      state[register_1]--;
    };
  } else if (split_line[0] == "jnz") {
    instr.type = InstructionType::JNZ;
    std::variant<int, char> var_val_1;
    std::variant<int, char> var_val_2;
    if (char(split_line[1][0]) >= 'a' && char(split_line[1][0]) <= 'd') {
      var_val_1  = char(split_line[1][0]);
    } else {
      var_val_1 = sv_to_int(split_line[1]);
    }
    if (char(split_line[2][0]) >= 'a' && char(split_line[2][0]) <= 'd') {
      var_val_2  = char(split_line[2][0]);
    } else {
      var_val_2 = sv_to_int(split_line[2]);
    }
    instr.process = [var_val_1, var_val_2](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {
      const bool first_is_not_zero = ((std::holds_alternative<char>(var_val_1) ? state[std::get<1>(var_val_1) - 'a'] : std::get<0>(var_val_1)) != 0);
      if (first_is_not_zero) {
        // std::cout << (std::holds_alternative<char>(var_val_2) ? (std::get<1>(var_val_2) - 'a'): std::get<0>(var_val_2)) << '\n';
        instr_ptr += (std::holds_alternative<char>(var_val_2) ? state[std::get<1>(var_val_2) - 'a']: std::get<0>(var_val_2));
      } else {
        instr_ptr++;
      }
    };
  } else if(split_line[0] == "tgl") {
    instr.type = InstructionType::TGL;
    if (char(split_line[1][0]) >= 'a' && char(split_line[1][0]) <= 'd') {
      const int register_1 = split_line[1][0] - 'a';
      instr.process = [register_1](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {        
        const int val = state[register_1] + instr_ptr;
        if(val < instrs.size()) {
          // std::cout << "toggling: " << val << '\n';
          instrs[val].toggled = !instrs[val].toggled;
        }
        instr_ptr++;
      };
    } else {
      const int val_1 = sv_to_int(split_line[1]);
      instr.process = [val_1](std::array<int, 4>& state, int& instr_ptr, std::vector<Instruction>& instrs) {        
        const int val = val_1 + instr_ptr;
        if(val < instrs.size()) {
          // std::cout << "toggling: " << val << '\n';
          instrs[val].toggled = !instrs[val].toggled;
        }
        instr_ptr++;
      };
    }
  } else {
    std::cout << "This should not happen" << '\n';
    exit(0);
  }
  return instr;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_23_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::vector<std::string> parsed_lines;
  std::vector<std::vector<std::string>> parsed_instructions;

  std::vector<Instruction> instructions;
  std::array<int, 4> registers {{12,0,0,0}};
  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    parsed_lines.push_back(line);
    parsed_instructions.emplace_back();
    auto& split_line = parsed_instructions.back();
    std::ranges::copy(parsed_lines.back() 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string{std::string_view(rng)};}), 
                            std::back_inserter(split_line));
    Instruction instr = createInstruction(split_line);
    if (instr.type == InstructionType::CPY) {
      split_line[0] = "jnz";
    }
    if (instr.type == InstructionType::INC) {
      split_line[0] = "dec";
    }
    if (instr.type == InstructionType::DEC) {
      split_line[0] = "inc";
    }
    if (instr.type == InstructionType::JNZ) {
      split_line[0] = "cpy";
    }
    if (instr.type == InstructionType::TGL) {
      split_line[0] = "inc";
    }
    const Instruction instr_toggled = createInstruction(split_line);
    instr.process_toggled = instr_toggled.process;
    instructions.push_back(instr);
  }

  int instr_ptr = 0;
  registers['c' - 'a'] = 1; 
  while(instr_ptr < instructions.size()) {
    instructions[instr_ptr].process_wrapper(registers, instr_ptr, instructions);
    // for (const auto& ele : registers) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << " | " << instr_ptr;
    // std::cout << '\n';
  }
  std::cout << registers[0] << '\n';
}