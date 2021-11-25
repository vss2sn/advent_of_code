#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <functional>
#include <sstream>


std::array<int, 4> parse_input(const std::string& s_main) {
  std::string s = s_main;
  std::array<int, 4> result;
  s.erase(std::remove(std::begin(s), std::end(s), ','), std::end(s));
  std::string delimiter = " ";
  std::size_t start = 0;
  auto end = s.find(delimiter);
  int count = 0;
  while (end != std::string::npos) {
    result[count] = std::stoi(s.substr(start, end - start));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
    count++;
  }
  result[count] = std::stoi(s.substr(start, s.size() - start));
  return result;
}

int main(int argc, char * argv []) {
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;

  std::array<int, 4> registers;
  std::array<std::function<void(const int, const int, const int)>, 16> functions {{
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] + registers[b]; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] + b; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] * registers[b]; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] * b; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] & registers[b]; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] & b; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] | registers[b]; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] | b; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a]; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = a; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = a > registers[b] ? 1 : 0; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] > b ? 1 : 0; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] > registers[b] ? 1 : 0; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = a == registers[b] ? 1 : 0; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] == b ? 1 : 0; } },
    { [&registers](const int a, const int b, const int c) { registers[c] = registers[a] == registers[b] ? 1 : 0; } }
  }};

  std::array<int, 4> expected_result;
  std::array<int, 4> init_state;
  std::array<int, 4> instr;

  int ans = 0;
  bool before_seen = false;
  int blank_line_count = 0;

  while(blank_line_count < 3 && std::getline(file, line)) {
    if (line[0] == 'B') {
      blank_line_count = 0;
      init_state = parse_input(line.substr(9, line.size() - 1 - 9));
      before_seen = true;
    } else if (!line.empty() && before_seen) {
      instr = parse_input(line);
      before_seen = false;
    } else if (line[0] == 'A') { // only one that does not call continue
      expected_result = parse_input(line.substr(9, line.size() - 1 - 9));
      int correct_outputs = 0;
      for (const auto& func : functions) {
        registers = init_state;
        func(instr[1], instr[2], instr[3]);
        if (registers == expected_result) {
          correct_outputs++;
        }
      }
      if (correct_outputs >= 3) {
        ans++;
      }
    } else {
      blank_line_count++;
    }
  }
  std::cout << ans << '\n';
  return 0;
}
