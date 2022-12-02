#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  int score = 0;
  while(std::getline(file, line)) {
    // Account for CRLF if required
    // line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line[2] == 'X') {
      score += (line[0] - 'A' - 1 + 3) % 3 + 1;
    } else if (line[2] == 'Y') {
      score += 3 + line[0] - 'A' + 1;
    } else {
      score += 6 + (line[0] - 'A' + 1) % 3 + 1;
    }
  }
  std::cout << score << '\n';
  return 0;
}
