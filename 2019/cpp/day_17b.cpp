#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <thread>

constexpr long long pow(const int base, const int raised_to) {
  long long ans = 1;
  for (int i = 1; i <= raised_to; i++) {
    ans *= base;
  }
  return ans;
}

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

class Program {
 public:
  Program(const std::vector<long long>& program) : init_program_state(program) {
    for (long long i = 0; i < program.size(); i++) {
      memory[i] = program[i];
    }
  }

  void InitIfNotInited(const long long key) {
    if (memory.find(key) == memory.end()) {
      memory[key] = 0;
    }
  }

  void reset() {
    for (long long i = 0; i < init_program_state.size(); i++) {
      memory[i] = init_program_state[i];
    }
    complete = false;
    inst_ptr = 0;
    relative_base = 0;
  }

  std::vector<long long> getNOps(const size_t n, const size_t inst_ptr,
                                 long long modes) {
    std::vector<long long> ops;
    for (size_t i = 1; i <= n; ++i) {
      size_t rem = modes % 10;
      modes = modes / 10;
      if (rem == 0) {
        InitIfNotInited(inst_ptr + i);
        InitIfNotInited(memory[inst_ptr + i]);
        ops.push_back(memory[memory[inst_ptr + i]]);
      } else if (rem == 1) {
        InitIfNotInited(inst_ptr + i);
        InitIfNotInited(memory[inst_ptr + i]);
        ops.push_back(memory[inst_ptr + i]);
      } else if (rem == 2) {
        InitIfNotInited(inst_ptr + i);
        InitIfNotInited(relative_base + memory[inst_ptr + i]);
        InitIfNotInited(memory[relative_base + memory[inst_ptr + i]]);
        ops.push_back(memory[relative_base + memory[inst_ptr + i]]);
      } else {
        // // std::cout << "Error" << '\n';
        exit(0);
      }
    }
    return ops;
  }

  long long getAddress(const size_t increment, long long modes) {
    long long address;
    const size_t rem = modes;
    if (rem == 0) {
      InitIfNotInited(inst_ptr + increment);
      address = memory[inst_ptr + increment];
    } else if (rem == 2) {
      InitIfNotInited(inst_ptr + increment);
      InitIfNotInited(relative_base + memory[inst_ptr + increment]);
      address = relative_base + memory[inst_ptr + increment];
    } else {
      // // std::cout << "Error" << '\n';
      exit(0);
    }
    return address;
  }

  long long compute(const std::vector<long long>& input) {
    is_input_requested = false;
    long long output = -1;
    size_t input_ptr = 0;
    while (memory[inst_ptr] != 99) {
      // std::cout << inst_ptr << '\n';
      const long long instr = memory[inst_ptr];
      const long long opcode = instr % 100;
      const long long modes = instr / 100;
      if (opcode == 1) {
        constexpr int n_ops = 2;
        const auto ops = getNOps(n_ops, inst_ptr, modes);
        memory[getAddress(3, modes / pow(10, n_ops))] = ops[0] + ops[1];
        inst_ptr += 4;
      } else if (opcode == 2) {
        constexpr int n_ops = 2;
        const auto ops = getNOps(n_ops, inst_ptr, modes);
        memory[getAddress(3, modes / pow(10, n_ops))] = ops[0] * ops[1];
        inst_ptr += 4;
      } else if (opcode == 3) {
        if (input.size() == input_ptr) {
          input_ptr = 0;
          is_input_requested = true;
          return -1;
        }
        memory[getAddress(1, modes)] = input[input_ptr];
        inst_ptr += 2;
        input_ptr += 1;
      } else if (opcode == 4) {
        const auto ops = getNOps(1, inst_ptr, modes);
        output = ops[0];
        inst_ptr += 2;
        // // // std::cout << output << '\n';
        return output;
      } else if (opcode == 5) {
        const auto ops = getNOps(2, inst_ptr, modes);
        if (ops[0] != 0) {
          inst_ptr = ops[1];
        } else {
          inst_ptr += 3;
        }
      } else if (opcode == 6) {
        const auto ops = getNOps(2, inst_ptr, modes);
        if (ops[0] == 0) {
          inst_ptr = ops[1];
        } else {
          inst_ptr += 3;
        }
      } else if (opcode == 7) {
        constexpr int n_ops = 2;
        const auto ops = getNOps(n_ops, inst_ptr, modes);
        if (ops[0] < ops[1]) {
          memory[getAddress(3, modes / pow(10, n_ops))] = 1;
        } else {
          memory[getAddress(3, modes / pow(10, n_ops))] = 0;
        }
        inst_ptr += 4;
      } else if (opcode == 8) {
        constexpr int n_ops = 2;
        const auto ops = getNOps(n_ops, inst_ptr, modes);
        if (ops[0] == ops[1]) {
          memory[getAddress(3, modes / pow(10, n_ops))] = 1;
        } else {
          memory[getAddress(3, modes / pow(10, n_ops))] = 0;
        }
        inst_ptr += 4;
      } else if (opcode == 9) {
        const auto ops = getNOps(1, inst_ptr, modes);
        relative_base += ops[0];
        inst_ptr += 2;
      }
    }
    complete = true;
    return output;
  }

  bool isComplete() { return complete; }

  bool isProgramWaitingForInput() {
    return is_input_requested;
  }

  void modifyMemoryAddress(const long long address, const long long value) {
    memory[address] = value;
  }

 private:
  const std::vector<long long> init_program_state;
  std::unordered_map<long long, long long> memory;
  bool complete = false;
  size_t inst_ptr = 0;
  size_t relative_base = 0;
  bool is_input_requested = false;
};

class IntcodeComputer {
 public:
  void addProgram(const std::vector<long long>& program) {
    programs.emplace_back(program);
  }

  long long runProgram(const size_t index,
                       const std::vector<long long>& input) {
    return programs[index].compute(input);
  }

  void resetProgram(const size_t index) { programs[index].reset(); }

  void modifyProgramMemoryAddress(const size_t index, const long long address, const long long value) {
    programs[index].modifyMemoryAddress(address, value);
  }

  bool isLastComplete() { return programs.back().isComplete(); }

  bool isComplete(size_t index) { return programs[index].isComplete(); }

  void reset() { programs.clear(); }

  bool isProgramWaitingForInput(const size_t index) {
    return programs[index].isProgramWaitingForInput();
  }

 private:
  std::vector<Program> programs;
};

std::vector<std::pair<int, int>> getNeighbours(const std::pair<int, int>& current) {
  std::vector<std::pair<int, int>> neighbours;
  neighbours.emplace_back(current.first + 1, current.second);
  neighbours.emplace_back(current.first - 1, current.second);
  neighbours.emplace_back(current.first, current.second + 1);
  neighbours.emplace_back(current.first, current.second - 1);
  return neighbours;
}

void print(const std::pair<int, int>& p) {
  // std::cout << "(" << p.first << ", " << p.second << ")" << '\n';
}

bool outOfBounds(const std::pair<int, int>& p, const std::vector<std::vector<char>>& view) {
  return p.first < 0 || p.first >= view.size() || p.second < 0 || p.second >= view[0].size();
}

void PrintGrid(const std::vector<std::vector<char>>& view) {
  for(const auto & row : view) {
    for (const auto c : row) {
      std::cout << c;
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

char convertMoveToChar(const std::pair<int, int>& move) {
  if (move == std::make_pair<int, int>(0, 1)) {
    return 'R';
  } else if (move == std::make_pair<int, int>(1, 0)) {
    return 'D';
  } else if (move == std::make_pair<int, int>(0, -1)) {
    return 'L';
  } else if (move == std::make_pair<int, int>(-1, 0)) {
    return 'U';
  }
  return 'N';
}

std::tuple<bool, std::pair<int, int>, char> getMomentumMove(const std::pair<int, int>& current,
  const std::pair<int, int>& previous,
  const std::vector<std::vector<char>>& view) {
  for (const auto& neighbour : getNeighbours(current)) {
    print(neighbour);
    if (outOfBounds(neighbour, view)) continue;
    if(view[neighbour.first][neighbour.second] != '#') continue;
    if(neighbour == previous) continue;
    const std::pair<int, int> move{neighbour.first - current.first, neighbour.second - current.second};
    const auto c = convertMoveToChar(move);
    return {true, move, c};
  }
  return {false, previous, 'N'};
}


std::string MomentumSearch(const std::pair<int, int>& robot_position, std::vector<std::vector<char>>& view) {
  std::string path_str;
  auto current = robot_position;
  std::pair<int, int> previous{view.size(), view[0].size()};
  auto [move_found, move, move_char] = getMomentumMove(current, previous, view);
  while (move_found) {
    int count = 0;
    std::pair<int, int> new_pos{current.first + move.first, current.second + move.second};
    while(!outOfBounds(new_pos, view) && view[new_pos.first][new_pos.second] == '#') {
      count++;
      new_pos.first = new_pos.first + move.first;
      new_pos.second = new_pos.second + move.second;
    }
    path_str += std::string(1, move_char) + "," + std::to_string(count) + ",";
    new_pos.first = new_pos.first - move.first;
    new_pos.second = new_pos.second - move.second;
    current = new_pos;
    new_pos.first = new_pos.first - move.first;
    new_pos.second = new_pos.second - move.second;
    previous = new_pos;
    view[current.first][current.second] = 'R';
    view[current.first][current.second] = '#';
    std::tie(move_found, move, move_char) = getMomentumMove(current, previous, view);
  }
  return path_str;
}

bool removeNOccuranceOfSubstr(std::string& str, const std::string& pattern, const int n) {
  for (size_t i = 0; i < n; i++) {
    std::string::size_type index = str.find(pattern);
    if (index == std::string::npos) return false;
    str.erase(index, pattern.length());
  }
  return true;
}

bool removeOneOccuranceOfSubstr(std::string& str, const std::string& pattern) {
  std::string::size_type index = str.find(pattern);
  if (index == std::string::npos) return false;
  str.erase(index, pattern.length());
  str.insert(index, " ");
  return true;
}

size_t getNthNextCutPoint(const std::string& path_str, size_t start, const int n) {
  int count = 0;
  while(count < n) {
    start = path_str.find(",", start) + 1;
    if(start == std::string::npos) return start;
    count++;
  }
  return start;
}

std::tuple<bool, std::vector<std::string>> SplitPathIntoNRepeatedStrings(const std::string& path_str, std::vector<std::string>& segments, const int n, int chars_used) {
  auto mod_path_str = path_str;
  while(!mod_path_str.empty() && mod_path_str[0] == ' ') mod_path_str.erase(0, 1);
  if (n == 0 && mod_path_str.empty()) return {true, segments};
  if (n == 0 && !mod_path_str.empty()) return {false, segments};
  if (chars_used == 20) return {false, segments};
  std::size_t start = 0;
  std::size_t end = getNthNextCutPoint(mod_path_str, 0, 2);
  auto init_chars_used = chars_used;
  while (end != std::string::npos) {
    auto current = mod_path_str;
    const std::string segment = mod_path_str.substr(0, end);
    if (segment.size() > 20) return {false, segments};
    segments.push_back(segment);
    chars_used = init_chars_used;
    int local_count = 0;
    while(removeOneOccuranceOfSubstr(current, segment)) {
      while(!current.empty() && current[0] == ' ') current.erase(0, 1);
      local_count++;
      chars_used += 2;
      auto [found, ret_segments] = SplitPathIntoNRepeatedStrings(current, segments, n - 1, chars_used);
      if (found) {
        return {found, ret_segments};
      }
      chars_used -= 2;
    }
    segments.pop_back();
    end = getNthNextCutPoint(mod_path_str, end + 1, 2);
  }
  return {false, segments};
}

std::vector<long long> convertToAscii(const std::string& str) {
  std::vector<long long> ret;
  for(const char c: str) {
    ret.push_back(int(c));
  }
  return ret;
}

std::string ConvertToRelativePath(const std::pair<int, int>& robot_position, std::vector<std::vector<char>>& view, const std::string& path) {
  std::string relative_path;
  int cur_dir = 0;
  if (view[robot_position.first][robot_position.second] == '^') {
    cur_dir = 0;
  } else if (view[robot_position.first][robot_position.second] == 'v') {
    cur_dir = 2;
  } else if (view[robot_position.first][robot_position.second] == '>') {
    cur_dir = 1;
  } else if (view[robot_position.first][robot_position.second] == '<') {
    cur_dir = 3;
  } else {
    exit(0);
  }
  for (int i = 0; i < path.size(); i++) {
    char char_to_append = path[i];
    if (path[i] == 'U') {
      auto prev_dir = cur_dir;
      cur_dir = 0;
      auto delta = (cur_dir - prev_dir + 4) % 4;
      if (delta == 3) char_to_append = 'L';
      else if (delta == 1) char_to_append = 'R';
      else exit(0);
    } else if (path[i] == 'R') {
      auto prev_dir = cur_dir;
      cur_dir = 1;
      auto delta = (cur_dir - prev_dir + 4) % 4;
      if (delta == 3) char_to_append = 'L';
      else if (delta == 1) char_to_append = 'R';
      else exit(0);
    } else if (path[i] == 'D') {
      auto prev_dir = cur_dir;
      cur_dir = 2;
      auto delta = (cur_dir - prev_dir + 4) % 4;
      if (delta == 3) char_to_append = 'L';
      else if (delta == 1) char_to_append = 'R';
      else exit(0);
    }
    else if (path[i] == 'L') {
      auto prev_dir = cur_dir;
      cur_dir = 3;
      auto delta = (cur_dir - prev_dir + 4) % 4;
      if (delta == 3) char_to_append = 'L';
      else if (delta == 1) char_to_append = 'R';
      else exit(0);
    }
    relative_path += std::string(1, char_to_append);
  }
  return relative_path;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_17_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_str;
  std::getline(file, input_str);
  const std::string delimiter = ",";

  std::size_t start = 0;
  std::size_t end = input_str.find(delimiter);
  std::vector<long long> program;
  while (end != std::string::npos) {
    program.emplace_back(std::stoll(input_str.substr(start, end - start)));
    start = end + delimiter.size();
    end = input_str.find(delimiter, start);
  }
  program.emplace_back(std::stoll(input_str.substr(start, end - start)));

  // Solve
  IntcodeComputer computer;
  computer.addProgram(program);
  std::pair<int, int> robot_position;
  int prev_output = 0;
  std::vector<std::vector<char>> view(1);
  int x = 0, y = 0;
  while(!computer.isComplete(0)) {
    const int output = computer.runProgram(0, {});
    if (output == 10) {
      if (output == prev_output) break;
      y += 1;
      x = 0;
      view.emplace_back();
    } else if (char(output) == '#' || char(output) == '.'){
      view.back().emplace_back(char(output));
      x+=1;
    } else {
      view.back().emplace_back(char(output));
      robot_position = std::make_pair(y, x);
      x+=1;
    }
    prev_output = output;
  }
  if(view.back().empty()) view.pop_back();

  const std::string path_str = MomentumSearch(robot_position, view);
  const std::string relative_path = ConvertToRelativePath(robot_position, view, path_str);
  std::vector<std::string> segments;
  SplitPathIntoNRepeatedStrings(relative_path, segments, 3, 0);
  std::swap(segments[1], segments[2]);
  std::string relative_path_copy = relative_path;
  std::vector<long long int> main_movement_routine;
  while (!relative_path_copy.empty()) {
    for (size_t i = 0; i < segments.size(); i++) {
      if (relative_path_copy.substr(0, segments[i].size()) == segments[i]) {
        main_movement_routine.push_back(int('A' + i));
        main_movement_routine.push_back(int(','));
        relative_path_copy.erase(0, segments[i].size());
        break;
      }
    }
  }

  main_movement_routine.pop_back();
  main_movement_routine.push_back('\n');
  program[0] = 2 ;
  computer.addProgram(program);
  long long output = 0;
  while (!computer.isProgramWaitingForInput(1)) {
    computer.runProgram(1, {});
  }
  output = computer.runProgram(1, main_movement_routine);
  while (!computer.isProgramWaitingForInput(1)) {
    computer.runProgram(1, {});
  }
  for(const auto& segment:segments) {
    auto temp = convertToAscii(segment);
    temp.pop_back();
    temp.push_back('\n');
    computer.runProgram(1, temp);
    while (!computer.isProgramWaitingForInput(1)) {
      computer.runProgram(1, {});
    }
  }
  computer.runProgram(1, {int('n'), '\n'});
  while (!computer.isLastComplete()) {
    auto prev_output = output;
    output = computer.runProgram(1, {});
    if (output == -1) output = prev_output; // -1 indicates input required, not an actual output in this implementation
  }
  std::cout << output << '\n';
  return output;
}
