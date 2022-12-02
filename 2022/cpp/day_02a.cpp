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
    score += line[2] - 'X' + 1;
    if (const auto delta = (line[2] - line[0] - 23 + 3) % 3; delta == 1) {
      score += 6;
    } else if (delta == 0) {
      score += 3;
    }
  }
  std::cout << score << '\n';
  return 0;
}
