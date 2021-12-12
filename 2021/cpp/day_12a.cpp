#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

void print(const std::vector<std::string>& v) {
  for (const auto& s : v) {
    std::cout << s << ' ';
  }
  std::cout << '\n';
}

void recurse (const std::string& s,
  std::unordered_set<std::string>& seen,
  const std::unordered_map<std::string, std::vector<std::string>>& map,
  long long& count) {
  if (s == "end") {
    count++;
    return;
  }
  if (s[0] > 97 && s[0] <=122) {
    seen.insert(s);
  }
  const auto it  = map.find(s);
  for (const auto& next : it->second) {
    if (seen.find(next) == seen.end()) {
      recurse(next, seen, map, count);
    }
  }
  seen.erase(s);
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::unordered_map<std::string, std::vector<std::string>> map;
  while(std::getline(file, line)) {
    const auto d = line.find('-');
    const auto p1 = line.substr(0, d);
    const auto p2 = line.substr(d + 1, line.size() - d - 1);
    if (map.find(p1) == map.end()) {
      map[p1] = std::vector<std::string>();
    }
    if (map.find(p2) == map.end()) {
      map[p2] = std::vector<std::string>();
    }
    map[p1].emplace_back(p2);
    map[p2].emplace_back(p1);
  }

  long long count = 0;
  std::unordered_set<std::string> seen;
  recurse("start", seen, map, count);
  std::cout << count << '\n';
  return 0;
}
