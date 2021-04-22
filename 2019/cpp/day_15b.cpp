#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>  
#include <vector>
#include <queue>
#include <stack>

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


struct Point2D {
  int x, y;
  Point2D(const int x = 0, const int y = 0) : x(x), y(y){}
  bool operator == (const Point2D& p) const {
    return (this->x == p.x &&  this->y == p.y);
  }
};

bool ComparePts(const Point2D& p1, const Point2D& p2) {
  return (p1.x == p2.x &&  p1.y == p2.y);
}

Point2D AddPoints(const Point2D& p1, const Point2D& p2) {
  return Point2D(p1.x + p2.x, p1.y + p2.y);
}
Point2D SubtractPoints(const Point2D& p1, const Point2D& p2) {
  return Point2D(p1.x - p2.x, p1.y - p2.y);
}

struct hashp2d {
  size_t operator()(const Point2D& p) const {
    return std::hash<int>{}(p.x) ^ std::hash<int>{}(p.y);
  }
};

std::vector<Point2D> getNeighbours(const Point2D& p) {
  std::vector<Point2D> deltas;
  deltas.emplace_back(AddPoints(p, Point2D(0, 1)));
  deltas.emplace_back(AddPoints(p, Point2D(0, -1)));
  deltas.emplace_back(AddPoints(p, Point2D(1, 0)));
  deltas.emplace_back(AddPoints(p, Point2D(-1, 0)));
  return deltas;
}

int convertMoveToInstruction (const Point2D& p) {
  if( ComparePts(p, Point2D(0, 1))) {
    return 1;
  }
  if (ComparePts(p, Point2D(1, 0))) {
    return 4;
  }
  if (ComparePts(p, Point2D(-1, 0))) {
    return 3;
  }
  if (ComparePts(p, Point2D(0, -1))) {
    return 2;
  }
  return 0;
}

std::vector<Point2D> dijkstra(std::unordered_map<Point2D, int, hashp2d>& grid,
  const Point2D& start,
  const Point2D& dest) {
  std::vector<Point2D> moves;
  std::queue<Point2D> points;
  std::unordered_set <Point2D, hashp2d> visited;
  std::unordered_map <Point2D, Point2D, hashp2d> parent;
  points.push(start);
  // if (grid.find(start) != grid.end() && grid[start] == 0) CallError("Start is obstacle"); // Obstacle
  while (!points.empty() && !ComparePts(points.front(), dest)) {
    const auto current = points.front();
    points.pop();
    visited.insert(current);
    bool found = false; // Early break condition
    for (auto& neighbour : getNeighbours(current)) {
      if (visited.find(neighbour) != visited.end()) continue; // Already popped
      if (grid.find(neighbour) != grid.end() && grid[neighbour] == 0) continue; // Obstacle
      if (grid.find(neighbour) == grid.end() && !ComparePts(neighbour, dest)) continue; // Unknown point. Search shoudl be restricted to known points
      parent[neighbour] = current;
      if (ComparePts(neighbour, dest)) {
        found = true;
        points.push(neighbour);
        break;
      }
      points.push(neighbour);
    }
    if (found) break;
  }
  auto current = dest;
  while(!ComparePts(current, start)) {
    moves.push_back(SubtractPoints(current, parent[current]));
    current = parent[current];
  }
  std::reverse(std::begin(moves), std::end(moves));
  return moves;
}

long long fillArea(const std::unordered_map<Point2D, int, hashp2d>& grid, const Point2D& start) {
  long long time_step = 0;
  std::queue<Point2D> next_points;
  std::unordered_set<Point2D, hashp2d> visited;
  next_points.push(start);
  while(!next_points.empty()) {
    long long to_expand = next_points.size();
    time_step++;
    for (long long i_to_expand = 0; i_to_expand < to_expand; i_to_expand++) {
      const auto current = next_points.front();
      visited.insert(current);
      next_points.pop();
      for(const auto& neighbour : getNeighbours(current)) {
        if (const auto it = grid.find(neighbour); it != grid.end() &&
            it->second != 0 &&
            visited.find(neighbour) == visited.end()) {
          next_points.push(neighbour);
        }
      }
    }
  }
  std::cout << time_step - 1 << '\n'; // Code assumes oxygen system location filled at time step 1
  return time_step - 1;
}

void exploreAndFill(IntcodeComputer& computer) {
  int output = 1;
  Point2D current(0, 0);
  Point2D oxygen_system_location;
  std::stack<Point2D> unknown;
  std::unordered_set <Point2D, hashp2d> visited;

  for (const auto& neighbour : getNeighbours(current)) {
    unknown.push(neighbour);
  }
  std::unordered_map<Point2D, int, hashp2d> grid;
  grid[current] = 1;

  while (output != 2) {
    const auto nearest_unknown_pt = unknown.top();
    unknown.pop();
    if(grid.find(nearest_unknown_pt) != grid.end() && grid[nearest_unknown_pt] == 0) continue;
    if(ComparePts(current, nearest_unknown_pt)) continue;
    if (grid.find(current) != grid.end() && grid[current] == 0) continue;
    // Search will never go through an obstacle, only the dest pount can be an obstacle, but is at yet unknow
    const auto moves = dijkstra(grid, current, nearest_unknown_pt);
    for (const auto& move : moves) {
      current = AddPoints(current, move);
      output = computer.runProgram(0, {convertMoveToInstruction(move)});
    }
    grid[nearest_unknown_pt] = output;
    if (output == 0) {
      current = SubtractPoints(current, moves.back());
    } else if (output == 1) {
      for (const auto& neighbour : getNeighbours(current)) {
        if(grid.find(neighbour) == grid.end()) unknown.push(neighbour);
      }
    } else if (output == 2) {
      oxygen_system_location = nearest_unknown_pt;
      for (const auto& neighbour : getNeighbours(current)) {
        if(grid.find(neighbour) == grid.end()) {
          unknown.push(neighbour);
        }
      }
    }
  }
  fillArea(grid, oxygen_system_location);
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_15_input";
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
  exploreAndFill(computer);

}
