#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>    
#include <cassert>

// Idea: 
// find the step at which the goal (node ending with Z) is first detected (offset/phase of loop)
// find the number of steps it takes to reach a previously seen (node, instruction) pair again (period of loop)
// Use the Extended Eulicidean Algorithm to find the lcm with starting offset of the above
// Extended Euclidian Algorithm implementaion based on 
// https://math.stackexchange.com/questions/2218763/how-to-find-lcm-of-two-numbers-when-one-starts-with-an-offset

std::pair<long long, long long> divmod (const long long a, const long long b) {
  return {a / b, a % b};
}

std::pair<long long, long long> extended_gcd(const long long a, const long long b) {
    long long old_r = a;
    long long r = b;
    long long old_s = 1; 
    long long s = 0;
    long long old_t = 0;
    long long t = 1;
    long long temp;
    while (r != 0) {
        const auto [quotient, remainder] = divmod(old_r, r);
        old_r = r;
        r = remainder;
        temp = old_s;
        old_s = s;
        s = temp - quotient * s;
        temp = old_t;
        old_t = t;
        t = temp - quotient * t;
    }
    return {old_r, old_s};
}
    

std::pair<long long, long long> combine_phased_rotations(const long long a_period, const long long a_phase, const long long b_period, const long long b_phase) {
  const auto [gcd, s] = extended_gcd(a_period, b_period);
  const auto phase_difference = a_phase - b_phase;
  const auto [pd_mult, pd_remainder] = divmod(phase_difference, gcd);
  if (pd_remainder != 0) {
      std::cout << "Rotation reference points never synchronize." << '\n';
      exit(0);
  }
  const long long combined_period = (a_period / gcd) * b_period;
  const long long combined_phase = (a_phase - s * pd_mult * a_period) % combined_period;
  return {combined_period, combined_phase};
}

struct State {
  std::string node;
  int idx;
  int n_steps;

  // For loop detection, state should be seen at same instruction (index with instructions;  instructions = [LR...])
  bool operator == (const State& s) const {
    return node == s.node && idx == s.idx;
  }
};

struct Hasher {
  long long operator ()(const State& s) const {
    return s.idx;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::string instructions;
  std::getline(file, instructions);
  std::unordered_map<std::string, std::array<std::string, 2>> map;
  const std::regex mask_pattern(R"(([0-9A-Z]+) = \(([0-9A-Z]+), ([0-9A-Z]+)\))");
  std::smatch mask_match;
  std::vector<std::string> nodes;
  while(std::getline(file, line)) {
    std::regex_search(line, mask_match, mask_pattern);
    map[mask_match[1]] = {{mask_match[2], mask_match[3]}};
  }

  for (const auto& [node, options] : map) {
    if (node[node.size()-1] == 'A') {
      // std::cout << node << '\n';
      nodes.push_back(node);
    }
  }
  
  std::unordered_set<State, Hasher> seen_map_prime;
  std::vector<long long> offset; // Kept for sanity check; will use offset at which node ending with Z was first seen as offset of loop
  std::vector<long long> loop_size;
  std::vector<std::pair<std::string, long long>> nodes_ending_in_Z;
  for (const auto& node : nodes) {
    // std::cout << "-----------------" << '\n';
    long long count_z = 0;
    auto seen_map = seen_map_prime;
    State current;
    current.node = node;
    current.idx = 0;
    current.n_steps = 0;
    int idx = 0;
    long long n_steps = 0;
    while (true) {
      if (current.node[current.node.size() - 1] == 'Z') {
        nodes_ending_in_Z.emplace_back(current.node, current.n_steps);
        count_z++;
      }
      if (seen_map.find(current) != seen_map.end()) {
        // std::cout << "Seen: " << current.node << '\n';
        const auto& original = *seen_map.find(current);
        offset.push_back(original.n_steps);
        loop_size.push_back(n_steps - original.n_steps);
        break;
      }
      seen_map.insert(current);
      const int turn = (instructions[idx] == 'L' ? 0 : 1);
      idx++;
      if (idx == instructions.size()) {
        idx = 0;
      }
      n_steps++;
      current.node = map[current.node][turn];
      current.n_steps = n_steps;
      current.idx = idx;
    }
    // assert(count_z == 1); // Assumes only 1 Z encountered in loop
    // std::cout << "Offset of loop: " << offset.back() << '\n' << "Loop size: " <<  loop_size.back() << '\n';
    // std::cout << "Node ending with Z: " << nodes_ending_in_Z.back().first << '\n' << "Location of node ending with z: " << nodes_ending_in_Z.back().second << '\n';    
    // std::cout << '\n';
  }

  long long current_period = loop_size[0];
  long long current_phase = nodes_ending_in_Z[0].second; // This is the point at which node ending with Z was first seen
  for (int i = 1; i < nodes_ending_in_Z.size(); i++) {
    const auto [new_period, new_phase] = combine_phased_rotations(current_period, current_phase, loop_size[i], nodes_ending_in_Z[i].second);
    current_period = new_period;
    current_phase = new_phase;
  }
  
  std::cout << current_period << '\n';
}

