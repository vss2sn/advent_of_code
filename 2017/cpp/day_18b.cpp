#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <queue> 

// HINT: When the puzzle says X or Y, unless it specifies register, X or Y can be a number
// jgz 1 3 is a possibility
// add p b is a possibility

enum class InstructionType{
  UNKNOWN, SND, SET, ADD, MUL, MOD, RCV, JGZ
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
  if (instr == "snd") {
    return InstructionType::SND;
  } else if (instr == "set") {
    return InstructionType::SET;
  } else if (instr == "add") {
    return InstructionType::ADD;
  } else if (instr == "mul") {
    return InstructionType::MUL;
  } else if (instr == "mod") {
    return InstructionType::MOD;
  } else if (instr == "rcv") {
    return InstructionType::RCV;
  } else if (instr == "jgz") {
    return InstructionType::JGZ;
  }
  std::cout << "UNKNOWN" << '\n';
  exit(0);
  return InstructionType::UNKNOWN;
}

struct Program {
  Program(const int id) : id(id) {
    for (char ch = 'a'; ch <= 'z'; ch++) {
      regs[ch] = 0;
      // std::cout << ch << ' ' << regs[ch] << '\n';
    }
    regs['p'] = id;
    // std::cout << 'p' << ' ' << regs['p'] << '\n' << '\n';
  }
  int id;
  std::queue<int> q;
  std::size_t instruction_ptr = 0;
  std::unordered_map<char, long long> regs;
  bool stuck = false;
  bool complete = false;

  std::pair<bool, long long> execute(const std::vector<Instruction>& instructions) {
    if (complete) {
      return {false,0};
    }
    if (instruction_ptr >= instructions.size()) {
      complete = true;
      std::cout << "Complete" << '\n';
      return {false,0};
    }
    const auto& instruction = instructions[instruction_ptr];
    // std::cout << id<< ": " << instruction_ptr << " -> " << instruction.line << '\n'; 
    if (instruction.type == InstructionType::SND) {
      instruction_ptr++;
      // if (regs[instruction.reg()] >= 0) {
        return {true, instruction.get_value(0, regs)};
      // }
    } else if (instruction.type == InstructionType::SET) {
      regs[instruction.reg()] = instruction.get_value(1, regs);
      instruction_ptr++;
    } else if (instruction.type == InstructionType::ADD) {
      regs[instruction.reg()] += instruction.get_value(1, regs);
      instruction_ptr++;
    } else if (instruction.type == InstructionType::MUL) {
      regs[instruction.reg()] *= instruction.get_value(1, regs);
      instruction_ptr++;
    } else if (instruction.type == InstructionType::MOD) {
      regs[instruction.reg()] = regs[instruction.reg()] % instruction.get_value(1, regs);
      instruction_ptr++;
    } else if (instruction.type == InstructionType::RCV) {
      if(!q.empty()) {
        regs[instruction.reg()] = q.front();
        q.pop();
        stuck = false;
        // std::cout << id <<": Un Stuck " << '\n';
        instruction_ptr++;
      } else {
        // std::cout << id <<": Stuck " << '\n';
        stuck = true;
      }
    } else if (instruction.type == InstructionType::JGZ) {
      // std::cout << instruction.get_value(0, regs) << '\n'; 
      if(instruction.get_value(0, regs) > 0) {
        // std::cout << instruction.get_value(1, regs) << '\n';
        // std::cout << "Jump: " << instruction_ptr << ": " << instructions[instruction_ptr].line << " ---> "; 
        instruction_ptr += instruction.get_value(1, regs);
        // std::cout << instruction_ptr << ": " << instructions[instruction_ptr].line << '\n'; 
      } else {
        instruction_ptr++;
      }
    } else {
      std::cout << "UNKNOWN" << '\n';
      exit(0);
    }
    // std::cout << id << ": ";
    // for (char ch = 'a'; ch <= 'z'; ch++) {
    //   if (regs[ch] != 0) {
    //     std::cout << '(' << ch << "," << regs[ch] << ')' << ' ';
    //   }
    // }
    // std::cout << '\n';
    return {false, 0};
  }
};

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
      // std::cout << "Args 1 is a register: " << char(line[4]) <<'\n';
      i.args[0] = char(line[4]);
      next = 6; 
    } else {
      next = line.find(' ', 4);
      if (next == std::string::npos) {
        // std::cout << "npos" << '\n';
        next == line.size();
      }
      // std::cout << "Args 1 is a number: " << std::stoi(line.substr(4, next - 4)) << '\n';
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
        // std::cout << "Args 2 is a register: " << char(line[next]) <<'\n';
      } else {
        // std::cout << "Args 2 is a number: " << std::stoi(line.substr(next, line.size() - next)) << '\n';
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
  std::vector<Program> programs = {Program(0), Program(1)};
  const std::vector<Instruction> instructions = parse_instructions(file); 
  std::size_t ans = 0; 
  while (!(programs[0].stuck || programs[0].complete) || !(programs[1].stuck || programs[1].complete)) {
    // std::cout << programs[0].stuck << programs[1].stuck << programs[0].complete << programs[1].complete << '\n';
    // if ((programs[0].stuck && programs[1].stuck)) {
    //   std::cout << "both stuck" << '\n';
    // } 
    // if (programs[0].complete && programs[1].complete) {
    //   std::cout << "both complete" << '\n';
    // }
    std::pair<bool, long long> result_0 = programs[0].execute(instructions);
    std::pair<bool, long long> result_1 = programs[1].execute(instructions);
    if (result_0.first) {
      programs[1].q.push(result_0.second);
      programs[1].stuck = false;
    } 
    if (result_1.first) {
      ans++;
      programs[0].q.push(result_1.second);
      programs[0].stuck = false;
    }
  }
  // std::cout << programs[0].stuck << programs[1].stuck << programs[0].complete << programs[1].complete << '\n';
  
  std::cout << ans << '\n';
  return 0;
}