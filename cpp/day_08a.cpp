#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <string_view>
#include <vector>

int main() {
  std::ifstream file{"../input/day_8_input"};
  std::vector<std::string> code_lines;
  std::string line;
  while (std::getline(file, line)) {
    code_lines.emplace_back(line);
  }
  std::vector<bool> executed(code_lines.size(), false);

  int line_n = 0;
  int acc = 0;
  while(true) {
    std::cout << "Instruction #" << line_n << '\n';
    if(executed[line_n]) break;
    const std::string_view inst = code_lines[line_n].substr(0,3);
    executed[line_n] = true;
    if(inst == "nop") {
      ++line_n;
    } else if (inst == "acc") {
      acc += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
      ++line_n;
    } else if (inst == "jmp") {
      line_n += stoi(code_lines[line_n].substr(4, code_lines[line_n].size() - 4));
    }
  }

  std::cout << acc << '\n';
  return acc;
}
