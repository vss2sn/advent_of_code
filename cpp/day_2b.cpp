#include <algorithm>
// #include <iostream>
#include <fstream>
#include <unordered_set>

int main() {
  std::ifstream file;
  file.open("day_2_data.txt");
  std::unordered_set<int> entries;
  char dash, letter, colon;
  std::string pwd;
  int i1, i2;
  int count = 0;
  while(file >> i1 >> dash >> i2 >> letter >> colon >> pwd) {
    const bool valid = (pwd[i1 - 1] == letter) ^ (pwd[i2 - 1] == letter);
    // std::cout << (valid ? "valid" : "invalid") << '\n';
    if (valid) {
      ++count;
    }
  }
  // std::cout << count << '\n';
  return count;
}
