#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  int x = 0;
  int d = 0;
  int aim = 0;
  while(std::getline(file, line)) {
    if (line[0] == 'f') {
      const int X = std::stoi(line.substr(7, line.size() - 7));
      x += X;
      d += aim * X;
    } else if (line[0] == 'd'){
      aim += std::stoi(line.substr(4, line.size() - 4));
    } else if (line[0] == 'u') {
      aim -= std::stoi(line.substr(2, line.size() - 2));
    }
  }
  std::cout << x * d << '\n';
  return 0;
}
