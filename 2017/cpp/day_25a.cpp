#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cmath>
#include <queue>
#include <utility>  
#include <cassert>   

struct Instruction {
  bool write_value;
  int move;
  std::string new_state_name;
};

struct State {
  std::string name;
  std::array<Instruction, 2> instructions;
};

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_25_input" ;  
  std::ifstream file(input);
  std::string line;
  std::vector<State> states;
  std::getline(file, line);
  std::string current_state_name = line.substr(15, line.size() - 15 - 1);
  std::getline(file, line);
  const int n_steps = std::stoi(line.substr(36, line.find(' ', 37)));
  std::getline(file, line);

  const auto get_instruction = [&file, &line]() {
    Instruction i;
    int idx = std::stoi(line.substr(26, line.size() - 26 - 1));
    std::getline(file, line);
    i.write_value = bool(std::stoi(line.substr(22, line.size() - 22 - 1)));
    std::getline(file, line);
    i.move = line[27] == 'r' ? 1 : -1;
    std::getline(file, line);
    i.new_state_name = line.substr(26, line.size() - 26 - 1);
    return std::pair<int, Instruction>{idx, i};
  };
  while(std::getline(file, line)) {
    State s;
    s.name = line.substr(9, line.size() - 9 - 1);
    std::getline(file, line);
    const auto [idx0, instruction0] = get_instruction();
    std::getline(file, line);
    const auto [idx1, instruction1] = get_instruction();
    std::getline(file, line);
    s.instructions[idx0] = instruction0;
    s.instructions[idx1] = instruction1;
    states.push_back(s);
  }

  const auto get_state = [&states](const std::string& state_name) {
    // assert(std::find_if(std::begin(states), std::end(states), [&state_name](const State& s) {return s.name == state_name;}) != states.end());
    return *std::find_if(std::begin(states), std::end(states), [&state_name](const State& s) {return s.name == state_name;});
  };

  std::unordered_map<int, bool> tape;
  int cursor_idx = 0;
  for (int step = 0; step < n_steps; step++) {
    const State& state = get_state(current_state_name);
    const auto it = tape.find(cursor_idx);
    const auto value = it == tape.end() ? 0 : 1;
    const auto i = state.instructions[value];
    if (i.write_value == 0) {
      if (value) tape.erase(it);
    } else {
      tape[cursor_idx] = 1;
    }
    cursor_idx += i.move;
    current_state_name = i.new_state_name;
  }
  std::cout << tape.size() << '\n';
  return 0;
}