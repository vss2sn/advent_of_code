#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <queue> 

// HINT: When the puzzle says X or Y, unless it specifies register, X or Y can be a number
// JNZ 1 3 is a possibility
// add p b is a possibility

enum class InstructionType{
  UNKNOWN, SND, SUB, SET, ADD, MUL, MOD, RCV, JGZ
};

struct Instruction {
  std::string line;
  InstructionType type;
  std::array<std::variant<long long, char>,2> args;

  long long reg() const {
    return std::get<char>(args[0]);
  }

  long long get_value (const int idx, std::unordered_map<char, long long>& regs) const {
    return args[idx].index() == 0 ? std::get<long long>(args[idx]) : regs[std::get<char>(args[idx])];
  }
};

InstructionType parse_type(const std::string& instr) {
  if (instr == "set") {
    return InstructionType::SET;
  } else if (instr == "sub") {
    return InstructionType::SUB;
  } else if (instr == "mul") {
    return InstructionType::MUL;
  } else if (instr == "jnz") {
    return InstructionType::JGZ;
  } 
  std::cout << "UNKNOWN" << '\n';
  exit(0);
  return InstructionType::UNKNOWN;
}

std::vector<Instruction> parse_instructions (std::ifstream& file) {
  std::vector<Instruction> instructions;
  std::string line;
  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    Instruction i;
    i.line = line;
    i.type = parse_type(line.substr(0, 3));
    int next = 0;
    if (line[4] >= 'a' && line[4] <='z') {
      std::cout << "Args 1 is a register: " << char(line[4]) <<'\n';
      i.args[0] = char(line[4]);
      next = 6; 
    } else {
      next = line.find(' ', 4);
      if (next == std::string::npos) {
        // std::cout << "npos" << '\n';
        next == line.size();
      }
      std::cout << "Args 1 is a number: " << std::stoi(line.substr(4, next - 4)) << '\n';
      i.args[0] = std::stoi(line.substr(4, next - 4));
      if (next != std::string::npos) {
        next++;
      }
    }
    // std::cout << line.size() << ' ' << next << '\n';
    if (line.size() >= next) {
      // std::cout << "Second args exists; size == " << line.size() << " > " << next << '\n';
      if (line[next] >= 'a' && line[next] <='z') {
        i.args[1] = char(line[next]);
        std::cout << "Args 2 is a register: " << char(line[next]) <<'\n';
      } else {
        std::cout << "Args 2 is a number: " << std::stoi(line.substr(next, line.size() - next)) << '\n';
        i.args[1] = std::stoi(line.substr(next, line.size() - next));
      }
    }
    
    instructions.push_back(i);
  }
  // exit(0);
  return instructions;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_18_input" ;  
  std::ifstream file(input);
  const std::vector<Instruction> instructions = parse_instructions(file); 
  std::unordered_map<char, long long> regs;
  for (char c = 'a'; c <= 'h'; c++) {
    regs[c] = 0;
  }
  std::size_t ans = 0; 
  std::size_t instruction_ptr = 0;
  while (instruction_ptr < instructions.size()) {
    const auto& instruction = instructions[instruction_ptr];
    if (instruction.type == InstructionType::SET) {
      regs[instruction.reg()] = instruction.get_value(1, regs);
      instruction_ptr++;
    } else if (instruction.type == InstructionType::SUB) {
      regs[instruction.reg()] -= instruction.get_value(1, regs);
      instruction_ptr++;
    } else if (instruction.type == InstructionType::MUL) {
      regs[instruction.reg()] *= instruction.get_value(1, regs);
      instruction_ptr++;
      ans++;
    } else if (instruction.type == InstructionType::JGZ) {
      if(instruction.get_value(0, regs) != 0) {
        instruction_ptr += instruction.get_value(1, regs);
      } else {
        instruction_ptr++;
      }
    } else {
      std::cout << "UNKNOWN" << '\n';
      exit(0);
    }
  }
  std::cout << ans << '\n';
  return 0;
}