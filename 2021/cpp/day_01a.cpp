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
  std::getline(file, line);
  int cur = std::stoi(line);
  int prev = cur;
  int count = 0;
  while(std::getline(file, line)) {
    prev = cur;
    cur = std::stoi(line);
    if (cur > prev) {
      count++;
    }
  }
  std::cout << count  << '\n';
  return 0;
}
