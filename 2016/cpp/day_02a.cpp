#include <array>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string instructions;

  constexpr int dim = 3;
  std::array<std::array<int, dim>, dim> keypad{
    std::array<int, 3>{1,2,3},
    std::array<int, 3>{4,5,6},
    std::array<int, 3>{7,8,9}
    };
  int current_x = 1;
  int current_y = 1;
  while(std::getline(file, instructions)) {
    for (const auto instruction : instructions) {
        if (instruction == 'U') {
            if (current_y != 0) current_y -= 1;
        }
        else if (instruction == 'R') {
            if (current_x != 2) current_x += 1;
        }
        else if (instruction == 'D') {
            if (current_y != 2) current_y += 1;
        }
        else if (instruction == 'L') {
            if (current_x != 0) current_x -= 1;
        }
        // std::cout << keypad[current_y][current_x];
    }
    std::cout << keypad[current_y][current_x];
  }
  std::cout << '\n';
  return 0;
}
