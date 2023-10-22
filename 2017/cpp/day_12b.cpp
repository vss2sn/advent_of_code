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

void add_pipe_to_map(const std::string& pipe, std::unordered_map<int, std::unordered_set<int>>& map) {
  std::size_t start = 0;
  const std::string delimiter = ", ";
  std::size_t end = pipe.find(' ');
  const int from_program = std::stoi(pipe.substr(start, end - start));
  start =  end + 1;
  start = pipe.find(' ', start) + 1;
  end = pipe.find(delimiter, start);
  while(end != std::string::npos) {
    const int to_program = std::stoi(pipe.substr(start, end - start));
    start = end + delimiter.size();
    end = pipe.find(delimiter, start);
    map[from_program].insert(to_program);
    map[to_program].insert(from_program);
  }
  const int to_program = std::stoi(pipe.substr(start, end - start));
  map[from_program].insert(to_program);
  map[to_program].insert(from_program);
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_12_input" ;  
  std::ifstream file(input);
  std::string line;
  std::unordered_map<int, std::unordered_set<int>> map;
  while(std::getline(file, line)) {
    add_pipe_to_map(line, map);
  }
  std::unordered_set<int> reachable_prime;
  int n_groups = 0;
  for (const auto& [start_program, reachables] : map) {
    if (reachable_prime.find(start_program) != reachable_prime.end()) continue;
    n_groups++;
    std::queue<int> to_expand;
    to_expand.push(start_program);
    // Using a new smaller set for efficiency
    std::unordered_set<int> reachable;
    while(!to_expand.empty()) {
      const auto& new_reachables = map[to_expand.front()];
      to_expand.pop();
      for (const auto& new_reachable : new_reachables) {
        if (reachable.find(new_reachable) != reachable.end()) {
          continue;
        }
        to_expand.push(new_reachable);
        reachable.insert(new_reachable);
        reachable_prime.insert(new_reachable);
      }
    }
  }
  std::cout << n_groups << '\n';
  return 0;
}