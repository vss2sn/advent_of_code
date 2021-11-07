#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>

struct pair_hash{
  template <typename T1, typename T2>
  size_t operator ()(const std::pair<T1, T2>& p) const {
    return std::hash<T1>{}(p.first) ^ std::hash<T2>{}(p.second);
  }
};

int main(int argc, char* argv[]) {

  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  const std::regex pattern(R"(#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+))");
  std::unordered_map<std::pair<int, int>, int, pair_hash> coords;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    const long long x = stoll(match[2]);
    const long long y = stoll(match[3]);
    const long long l = stoll(match[4]);
    const long long b = stoll(match[5]);
    for (int i = 0; i < l; i++) {
      for (int j = 0; j < b; j++) {
        const auto p = std::make_pair(x+i, y+j);
        if (coords.find(p) == coords.end()) {
          coords[p] = 1;
        } else {
          coords[p]++;
        }
      }
    }
  }

  int count = 0;
  for(const auto& [key, val] : coords) {
    if (val > 1) {
      count++;
    }
  }
  std::cout << count << '\n';
  return count;
}
