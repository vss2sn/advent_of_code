#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <functional>

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

inline void execture_sample_input(std::array<std::vector<int>, 16>& opcode_to_function_map,
  const std::array<int, 4>& init_state,
  std::array<int, 4>& registers,
  const std::array<int, 4>& expected_result,
  const std::array<int, 4>& instr,
  const std::array<std::function<void(const int, const int, const int)>, 16>& functions) {
  std::vector<int> to_erase;
  for (const auto& index : opcode_to_function_map[instr[0]]) {
    registers = init_state;
    functions[index](instr[1], instr[2], instr[3]);
    if (registers != expected_result) {
      to_erase.push_back(index);
    }
  }
  for (auto it = std::rbegin(to_erase); it != std::rend(to_erase); it++) {
    opcode_to_function_map[instr[0]].erase(std::find(std::begin(opcode_to_function_map[instr[0]]), std::end(opcode_to_function_map[instr[0]]), *it));
  }
}

inline std::unordered_map<int, int> create_final_mapping(std::array<std::vector<int>, 16>& opcode_to_function_map) {
  std::unordered_map<int, int> final_mapping;
  while (std::find_if(std::begin(opcode_to_function_map), std::end(opcode_to_function_map), [](const std::vector<int>& v) {return v.size() > 0; })
  != std::end(opcode_to_function_map)) {
    auto opcode_with_only_one_option_it = std::find_if(std::begin(opcode_to_function_map), std::end(opcode_to_function_map), [](const auto& v) {return v.size() == 1;});
    const int index_to_be_removed = (*opcode_with_only_one_option_it)[0];
    final_mapping.insert({std::distance(std::begin(opcode_to_function_map), opcode_with_only_one_option_it), index_to_be_removed });
    for (auto& v : opcode_to_function_map) {
      auto it  = std::remove(std::begin(v), std::end(v), index_to_be_removed);
      if (it != v.end()) {
        v.erase(it, v.end());
      }
    }
  }
  return final_mapping;
}

int main(int argc, char * argv []) {
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;

  std::array<int, 4> registers;
  // Only for debug
  const std::array<std::string, 16> function_names {
    "addr",
    "addi",
    "mulr",
    "muli",
    "banr",
    "bani",
    "borr",
    "bori",
    "setr",
    "seti",
    "gtir",
    "gtri",
    "gtrr",
    "eqir",
    "eqri",
    "eqrr",
  };

  const std::array<std::function<void(const int, const int, const int)>, 16> functions {{
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

  std::vector<int> temp;
  for (int i = 0; i < functions.size(); i++) {
    temp.push_back(i);
  }

  std::array<std::vector<int>, 16> opcode_to_function_map;
  for (auto& ele : opcode_to_function_map) {
    ele = temp;
  }

  std::array<int, 4> expected_result;
  std::array<int, 4> init_state;
  std::array<int, 4> instr;

  bool before_seen = false;
  int blank_line_count = 0;
  // 3 for the black lines between the sample inputs and test program
  while(blank_line_count < 3 && std::getline(file, line)) {
    if (line[0] == 'B') {
      blank_line_count = 0;
      init_state = parse_input(line.substr(9, line.size() - 1 - 9));
      before_seen = true;
    } else if (line[0] == 'A') {
      expected_result = parse_input(line.substr(9, line.size() - 1 - 9));
      execture_sample_input(opcode_to_function_map, init_state, registers, expected_result, instr, functions);
    } else if (!line.empty() && before_seen) {
      instr = parse_input(line);
      before_seen = false;
    } else {
      blank_line_count++;
    }
  }

  const std::unordered_map<int, int> final_mapping = create_final_mapping(opcode_to_function_map);
  registers = {0,0,0,0};
  while(std::getline(file, line)) {
    instr = parse_input(line);
    functions[final_mapping.find(instr[0])->second](instr[1], instr[2], instr[3]);
  }
  std::cout << registers[0] << '\n';
  return 0;
}
