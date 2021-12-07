#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::pair<std::string, std::array<int, 3>> parse_input(const std::string& s_main) {
  std::string s = s_main;
  std::array<int, 3> nums;
  std::string delimiter = " ";
  std::size_t start = 0;
  auto end = s.find(delimiter);
  const auto instr = s.substr(start, end - start);
  start = end + 1;
  end = s.find(delimiter, start);
  int count = 0;
  while (end != std::string::npos) {
    nums[count] = std::stoi(s.substr(start, end - start));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
    count++;
  }
  nums[count] = std::stoi(s.substr(start, s.size() - start));
  return {instr, nums};
}

template<typename T>
void print(const T& t) {
  std::cout << "[";
  for (const auto& ele : t) {
    std::cout << ele << ' ';
  }
  std::cout << "]";
}

int main(int argc, char * argv []) {
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;

  std::array<int, 6> registers;
  const std::unordered_map<std::string, std::function<void(const int, const int, const int)>> map
   {
    {"addr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] + registers[b]; } } },
    {"addi", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] + b; } } },
    {"mulr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] * registers[b]; } } },
    {"muli", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] * b; } } },
    {"banr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] & registers[b]; } } },
    {"bani", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] & b; } } },
    {"borr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] | registers[b]; } } },
    {"bori", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] | b; } } },
    {"setr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a]; } } },
    {"seti", { [&registers](const int a, const int b, const int c) { registers[c] = a; } } },
    {"gtir", { [&registers](const int a, const int b, const int c) { registers[c] = a > registers[b] ? 1 : 0; } } },
    {"gtri", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] > b ? 1 : 0; } } },
    {"gtrr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] > registers[b] ? 1 : 0; } } },
    {"eqir", { [&registers](const int a, const int b, const int c) { registers[c] = a == registers[b] ? 1 : 0; } } },
    {"eqri", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] == b ? 1 : 0; } } },
    {"eqrr", { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] == registers[b] ? 1 : 0; } } }
  };


  std::getline(file, line);
  const auto instr_reg = line[4] - '0';
  std::vector<std::pair<std::string, std::array<int, 3>>> instrs;
  while(std::getline(file, line)) {
    instrs.emplace_back(parse_input(line));
  }
  registers = {0, 0, 0, 0, 0, 0};
  int instr_p = 0;
  while(instr_p < instrs.size()) {
    registers[instr_reg] = instr_p;

    const auto it = map.find(instrs[instr_p].first);
    it->second(instrs[instr_p].second[0], instrs[instr_p].second[1], instrs[instr_p].second[2]);

    instr_p = registers[instr_reg];
    instr_p++;
  }
  std::cout << registers[0] << '\n';

  return 0;
}
