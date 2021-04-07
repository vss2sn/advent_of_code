#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <unordered_set>

int main() {
  std::ifstream file;
  file.open("../input/day_6_input");

  int count = 0;
  std::string line;
  std::unordered_set<char> group_ans;

  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line) );
    if(line.empty()) {
      count += group_ans.size();
      group_ans.clear();
    } else {
      for(const char c : line) {
        group_ans.insert(c);
      }
    }
  }
  count += group_ans.size();
  group_ans.clear();

  std::cout << count << '\n';
  return count;
}
