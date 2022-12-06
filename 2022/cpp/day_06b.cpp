#include <array>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);

  constexpr int n = 14;
  int count = 0;
  std::array<int, 26> counter = {};
  // ALWAYS initialize std::array.

  for (int i = 0; i < n - 1; i++) {
    if (counter[line[i] - 'a'] == 0) count++;
    counter[line[i] - 'a'] += 1;
  }
  for (int i = n - 1; i < line.size(); i++) {
    if (counter[line[i] - 'a'] == 0) count++;
    counter[line[i] - 'a'] += 1;
    if (count == n) {
      std::cout << i + 1 << '\n';
      break;
    }
    counter[line[i- n + 1] - 'a'] -= 1;
    if (counter[line[i- n + 1] - 'a'] == 0) {
      count--;
    }
  }
  return 0;
}
