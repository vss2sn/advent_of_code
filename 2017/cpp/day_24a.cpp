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

struct Component {
  std::array<int, 2> ends;
  bool reverse = false;
  bool used = false;
  
  int first() {
    if (reverse) return ends[1];
    return ends[0]; 
  }

  int second() {
    if (reverse) return ends[0];
    return ends[1]; 
  }

  std::pair<bool, int> check_match(const int n) {
    if (n == ends[0]) return {true, 0};
    else if (n == ends[1]) return {true, 1};
    return {false, -1};
  }
};

void dfs (std::vector<Component>& components, const int pins, std::size_t& ans, std::size_t& current_total) {
  for (int c_idx = 0; c_idx < components.size(); c_idx++) {
    auto& c = components[c_idx];
    if (c.used) continue;
    if (const auto [match, idx] = c.check_match(pins); match) {
      if (idx == 1) {
        c.reverse = true;
      }
      c.used = true;
      current_total += c.first() + c.second();
      ans = std::max(ans, current_total);
      dfs(components, c.second(), ans, current_total);
      c.used = false;
      current_total -= (c.first() + c.second());
      if (idx == 1) {
        c.reverse = false;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_24_input" ;  
  std::ifstream file(input);
  std::string line;
  std::vector<Component> components;
  while(std::getline(file, line)) {
    Component c;
    const auto idx = line.find('/');
    c.ends[0] = std::stoi(line.substr(0, idx));
    c.ends[1] = std::stoi(line.substr(idx + 1, line.size() - idx - 1));
    components.push_back(c);
  }
  std::size_t ans;
  std::size_t current_total;
  dfs(components, 0, ans, current_total);
  std::cout << ans << '\n';
  return 0;
}