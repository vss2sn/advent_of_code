#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  int max = 0;
  int total = 0;
  while(std::getline(file, line)) {
    // Account for CRLF if required
    // line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line.size() == 0) {
      max = std::max(max, total);
      total = 0;
    } else {
      total += std::stoi(line);
    }
  }
  max = std::max(max, total);
  std::cout << max << '\n';
  return max;
}
