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

  constexpr int dim = 7;
  std::array<std::array<char, dim>, dim> keypad{
    std::array<char, dim>{'0', '0', '0', '0', '0', '0', '0'},
    std::array<char, dim>{'0', '0', '0', '1', '0', '0', '0'},
    std::array<char, dim>{'0', '0', '2', '3', '4', '0', '0'},
    std::array<char, dim>{'0', '5', '6', '7', '8', '9', '0'},
    std::array<char, dim>{'0', '0', 'A', 'B', 'C', '0', '0'},
    std::array<char, dim>{'0', '0', '0', 'd', '0', '0', '0'},
    std::array<char, dim>{'0', '0', '0', '0', '0', '0', '0'},
  };
  int current_x = 1;
  int current_y = 3;
  while(std::getline(file, instructions)) {
    for (const auto instruction : instructions) {
        if (instruction == 'U') {
            if (keypad[current_y - 1][current_x] != '0') current_y -= 1;
        }
        else if (instruction == 'R') {
            if (keypad[current_y][current_x + 1] != '0') current_x += 1;
        }
        else if (instruction == 'D') {
            if (keypad[current_y + 1][current_x] != '0') current_y += 1;
        }
        else if (instruction == 'L') {
            if (keypad[current_y][current_x - 1] != '0') current_x -= 1;
        }
    }
    std::cout << keypad[current_y][current_x];
  }
  std::cout << '\n';
  return 0;
}
