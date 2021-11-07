#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

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
  std::unordered_map<std::pair<int, int>, std::pair<int, int>, pair_hash> coords;
  std::vector<std::vector<int>> claims;
  std::smatch match;
  while(std::getline(file, line)) {
    std::regex_search(line, match, pattern);
    const int id = stoi(match[1]);
    const int x = stoi(match[2]);
    const int y = stoi(match[3]);
    const int l = stoi(match[4]);
    const int b = stoi(match[5]);
    claims.emplace_back(std::vector<int>{x, y, l, b});
    for (int i = 0; i < l; i++) {
      for (int j = 0; j < b; j++) {
        const auto p = std::make_pair(x+i, y+j);
        if (coords.find(p) == coords.end()) {
          coords[p] = {1, id};
        } else {
          coords[p].first++;
        }
      }
    }
  }

  std::unordered_set<int> ids;
  for(const auto& [key, val] : coords) {
    if (val.first == 1) {
      ids.insert(val.second);
    }
  }

  for(const auto id : ids) {
    const int x = claims[id][0];
    const int y = claims[id][1];
    const int l = claims[id][2];
    const int b = claims[id][3];
    bool all_one = true;
    for (int i = 0; i < l && all_one; i++) {
      for (int j = 0; j < b && all_one; j++) {
        const auto p = std::make_pair(x+i, y+j);
        if (coords[p].first != 1) {
          all_one = false;
        }
      }
    }

    if (all_one == true) {
      // Id and index in vector is offset by 1, ids start with 1, index with 0
      std::cout << id + 1 << '\n';
      return id + 1;
    }
  }
  return 0;
}
