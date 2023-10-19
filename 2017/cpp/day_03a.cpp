#include <fstream>
#include <iostream>
#include <string>
#include <charconv>
#include <limits>
#include <ranges>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_03_input" ;  
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  int number = std::stoi(line);
  int box_side = 1;
  if (number == 1) {
    std::cout << 0 << '\n';
    return 0;
  }
  while (number > (box_side * box_side)) {
    box_side += 2;
  }
  box_side -= 2;
  std::cout << "number: " << number << '\n';
  std::cout << "box_side: " << box_side << '\n';
  
  int distance = 0;
  const auto box_side_2 = box_side * box_side;
  if (number < box_side_2 + box_side + 1) {
    std::cout << __LINE__ << '\n';
    const auto x = box_side/2 + 1;
    const auto y = -box_side/2 + number - box_side_2 - 1;
    std::cout << "(" << x << ", " << y << ")" << '\n';
    distance = std::abs(x) + std::abs(y);
  }
  else if (number < box_side_2 + 2 * (box_side + 1)) {
    std::cout << __LINE__ << '\n';
    const auto x = (box_side/2 + 1) - (number - box_side_2 - box_side - 1);
    const auto y = box_side/2 + 1;
    std::cout << "(" << x << ", " << y << ")" << '\n';
    distance = std::abs(x) + std::abs(y);
  }
  else if (number < box_side_2 + 3 * (box_side + 1)) {
    std::cout << __LINE__ << '\n';
    const auto x = -(box_side/2 + 1);
    const auto y = (box_side/2 + 1) - (number - box_side_2 - 2 * (box_side + 1));
    std::cout << "(" << x << ", " << y << ")" << '\n';
    distance = std::abs(x) + std::abs(y);
  }
  else  {
    std::cout << __LINE__ << '\n';
    const auto x = -(box_side/2 + 1) +  (number - box_side_2 - 3 * (box_side + 1));
    const auto y = -(box_side/2 + 1);
    std::cout << "(" << x << ", " << y << ")" << '\n';
    distance = std::abs(x) + std::abs(y);
  }
  std::cout << distance << '\n';
  return 0;
}