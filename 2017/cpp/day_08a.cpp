#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>

bool evaluate_condition (std::unordered_map<std::string, int>& regs, const std::string& reg, const std::string& operation, const int value) {
  if (operation == ">=") return regs[reg] >= value;
  else if (operation == "<=") return regs[reg] <= value;
  else if (operation == ">") return regs[reg] > value;
  else if (operation == "<") return regs[reg] < value;
  else if (operation == "==") return regs[reg] == value;
  else if (operation == "!=") return regs[reg] != value;
  std::cout << "This should not happen. Unknown operation: " << operation << '\n';
  exit(0);
  return true;
}

void perform_action (std::unordered_map<std::string, int>& regs, const std::string& reg, const std::string& action, const int action_val) {
  if (action == "inc") {
    regs[reg] += action_val;
  } else if (action == "dec") {
    regs[reg] -= action_val;
  } else {
    std::cout << "This should not happen. Unknown action: " << action << '\n';
  }
}

struct Instruction {
  std::string reg;
  std::string action;
  int action_val;
  std::string condition_reg;
  std::string condition_operation;
  int condition_operation_value;
};

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_08_input" ;  
  std::ifstream file(input);
  std::string line;
  const std::regex pattern(R"(([a-z]+) ([a-z]+) (-?[0-9]+) if ([a-z]+) ([><=!]+) (-?[0-9]+))");
  std::unordered_map<std::string, int> regs;
  std::vector<Instruction> instructions;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    // std::cout << match[0] << ' '<< match[1] << ' '<< match[2] << ' '<< match[3] << ' '<< match[4] << ' '<< match[5] << ' '<< match[6] << '\n';
    Instruction instruction;
    instruction.reg = match[1];
    instruction.action = match[2];
    instruction.action_val = std::stoi(match[3]);
    instruction.condition_reg = match[4];
    instruction.condition_operation = match[5];
    instruction.condition_operation_value = std::stoi(match[6]);
    instructions.push_back(instruction);
    regs[match[1]] = 0;
    regs[match[4]] = 0;
  }

  for (const auto& instruction : instructions) {
    if(evaluate_condition(regs, instruction.condition_reg, instruction.condition_operation, instruction.condition_operation_value)) {
      perform_action(regs, instruction.reg, instruction.action, instruction.action_val);
    }
  }
  int max_val = std::numeric_limits<int>::min();
  for(const auto&[reg, val] : regs) {
    max_val = std::max(val, max_val);
  }
  std::cout << max_val << '\n';
  return 0;
}