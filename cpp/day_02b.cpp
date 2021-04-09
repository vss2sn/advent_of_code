#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_set>

int main() {
  std::ifstream file;
  file.open("../input/day_02_input");
  std::unordered_set<int> entries;
  char dash, letter, colon;
  std::string pwd;
  int i1, i2;
  int count = 0;
  while (file >> i1 >> dash >> i2 >> letter >> colon >> pwd) {
    const bool valid = (pwd[i1 - 1] == letter) ^ (pwd[i2 - 1] == letter);
    if (valid) {
      ++count;
    }
  }
  std::cout << count << '\n';
  return count;
}
