#include <fstream>
#include <iostream>
#include <unordered_set>

int main() {
  constexpr int total = 2020;
  std::ifstream file;
  file.open("../input/day_01_input");
  std::unordered_set<int> entries;
  int num = 0;
  while (file >> num) {
    const int delta = 2020 - num;
    if (entries.find(delta) != entries.end()) {
      std::cout << delta * num << '\n';
      return delta * num;
    } else {
      entries.insert(num);
    }
  }
  return 0;
}
