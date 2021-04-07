#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

std::tuple<bool, int> DetectLoop(std::vector<std::string>& code_lines,
                                 std::vector<bool> executed,  /* copy, not ref*/
                                 int line_n) {
  int acc = 0;
  bool loop_found = false;
  while(line_n < code_lines.size()) {
    if(executed[line_n]) {
      loop_found = true;
      break;
    }
    executed[line_n] = true;
    if(code_lines[line_n].substr(0,3) == "nop") {
      ++line_n;
    } else if (code_lines[line_n].substr(0,3) == "acc") {
      acc += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
      ++line_n;
    } else if (code_lines[line_n].substr(0,3) == "jmp") {
      line_n += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
    }
  }

  return {loop_found, acc};
}

int main() {
  std::ifstream file{"../input/day_8_input"};
  std::vector<std::string> code_lines;
  std::string line;
  while (std::getline(file, line)) {
    code_lines.emplace_back(line);
  }
  std::vector<bool> executed(code_lines.size(), false);

  int line_n = 0;
  int candidate_for_change = 0;
  int acc = 0;
  bool loop_found = false;

  while(line_n < code_lines.size()) {

    // Execute all acc instructions
    while(code_lines[line_n].substr(0,3) == "acc") {
      executed[line_n] = true;
      acc += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
      ++line_n;
    }

    // Inverse and execute instruction
    candidate_for_change = line_n;
    executed[line_n] = true;
    if(code_lines[line_n].substr(0,3) == "nop") {
      line_n += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
    } else if (code_lines[line_n].substr(0,3) == "jmp") {
      ++line_n;
    }

    if(std::tuple<bool, int> result = DetectLoop(code_lines, executed, line_n); !std::get<0>(result)) {
      executed[line_n] = true;
      acc += std::get<1>(result);
      break;
    }

    // Execute the instruction without inverting
    line_n = candidate_for_change;
    executed[line_n] = true;
    if(code_lines[line_n].substr(0,3) == "nop") {
      ++line_n;
    } else if (code_lines[line_n].substr(0,3) == "acc") {
      acc += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
      ++line_n;
    } else if (code_lines[line_n].substr(0,3) == "jmp") {
      line_n += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
    }
  }

  std::cout << acc << '\n';
  return acc;
}
