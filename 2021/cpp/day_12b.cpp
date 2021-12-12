#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

// Can use a tree to optimize this for the general case,
// or a counter instead of a bool

void print(const std::vector<std::string>& v) {
  for (const auto& s : v) {
    std::cout << s << ',';
  }
  std::cout << '\n';
}

void recurse (const std::string& s,
  std::unordered_map<std::string, int>& seen,
  const std::unordered_map<std::string, std::vector<std::string>>& map,
  bool seen_twice,
  long long& count) {
  if (s == "end") {
    count++;
    return;
  }
  seen[s]++;

  // If seeing a second lower case letter twice, return
  if (s[0] >= 97 && s[0] <=122 && seen[s] > 1 && seen_twice) {
    return;
  }

  // If seeing a second lower case letter, set seen_twice to true
  if (s[0] >= 97 && s[0] <=122 && seen[s] > 1) {
    seen_twice = true;
  }

  const auto it  = map.find(s);
  for (const auto& next : it->second) {
    if (!(next[0] >= 97 && next[0] <=122) || // uppercase
        (seen[next] < 1) || // lowercase but seen only once
        (next[0] >= 97 && next[0] <=122 && seen[next] == 1 && !seen_twice)) { // first time seeing a lowercase letter twice
      recurse(next, seen, map, seen_twice, count);
    }
  }
  seen[s]--;
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

  std::unordered_map<std::string, int> seen;
  for (const auto& p : map) {
    seen[p.first] = 0;
  }
  seen["start"] = 3;
  long long count = 0;

  const auto it  = map.find("start");
  for (const auto& next : it->second) {
    recurse(next, seen, map, false, count);
  }
  std::cout << count << '\n';
  return 0;
}
