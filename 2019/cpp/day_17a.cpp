#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

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
        std::cout << "Error" << '\n';
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
      std::cout << "Error" << '\n';
      exit(0);
    }
    return address;
  }

  long long compute(const std::vector<long long>& input) {
    long long output = -1;
    size_t input_ptr = 0;
    while (memory[inst_ptr] != 99) {
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
        memory[getAddress(1, modes)] = input[input_ptr];
        inst_ptr += 2;
        input_ptr += 1;
      } else if (opcode == 4) {
        const auto ops = getNOps(1, inst_ptr, modes);
        output = ops[0];
        inst_ptr += 2;
        // std::cout << output << '\n';
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

 private:
  const std::vector<long long> init_program_state;
  std::unordered_map<long long, long long> memory;
  bool complete = false;
  size_t inst_ptr = 0;
  size_t relative_base = 0;
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

  bool isLastComplete() { return programs.back().isComplete(); }

  bool isComplete(size_t index) { return programs[index].isComplete(); }

  void reset() { programs.clear(); }

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

bool outOfBounds(const std::pair<int, int>& p, const std::vector<std::vector<char>>& view) {
  return p.first < 0 || p.first >= view.size() || p.second < 0 || p.second >= view[0].size();
}

int GetAlignment (const std::pair<int, int>& robot_position, const std::vector<std::vector<char>>& view) {
  std::stack<std::pair<int, int>> positions;
  std::unordered_set<std::pair<int, int>, hash_pair> reached_before;
  for (const auto& neighbour : getNeighbours(robot_position)) {
    if (outOfBounds(neighbour, view)) continue;
    if (view[neighbour.first][neighbour.second] == '#') positions.push(neighbour);
  }

  int total = 0;
  int n_intersections = 0;
  while(!positions.empty()) {
    const auto new_point = positions.top();
    positions.pop();
    reached_before.insert(new_point);
    int count = 0;
    for (const auto& neighbour : getNeighbours(new_point)) {
      if (outOfBounds(neighbour, view)) {
        continue;
      }
      if (view[neighbour.first][neighbour.second] == '#') {
        count++;
        if (reached_before.find(neighbour) == reached_before.end()) {
          positions.push(neighbour);
        }
      }
    }
    if (count == 4) {
      total += (new_point.first * new_point.second);
    }
  }
  return total;
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
  auto ans = GetAlignment(robot_position, view);
  std::cout << ans << '\n';
  return ans;
}
