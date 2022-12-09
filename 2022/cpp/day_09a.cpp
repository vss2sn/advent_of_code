#include <array>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <string>

// TODO: Improve hash function

int main(int argc, char * argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::array<int, 2> point_H = {0, 0};
  std::array<int, 2> point_T = {0, 0};
  std::unordered_set<int> s;
  while(std::getline(file, line)) {
    for (int i = 0; i < std::stoi(line.substr(2, line.size() - 2)); i++) {
      if (line[0] == 'U') {
        point_H[0] += 1;
      } else if (line[0] == 'D') {
        point_H[0] -= 1;
      } else if (line[0] == 'R') {
        point_H[1] += 1;
      } else {
        point_H[1] -= 1;
      }

      const auto delta = std::array<int,2>{point_H[0] - point_T[0], point_H[1] - point_T[1]};
      if (std::abs(delta[0]) == 2 && std::abs(delta[1]) == 0) {
        point_T[0] += delta[0]/2;
      } else if (std::abs(delta[0]) == 2 && std::abs(delta[1]) == 1) {
        point_T[0] += delta[0]/2;
        point_T[1] += delta[1];
      } else if (std::abs(delta[0]) == 0 && std::abs(delta[1]) == 2) {
        point_T[1] += delta[1]/2;
      } else if (std::abs(delta[0]) == 1 && std::abs(delta[1]) == 2) {
        point_T[0] += delta[0];
        point_T[1] += delta[1]/2;
      }
      s.insert(point_T[0] * 100000 + point_T[1]);
    }
  }
  std::cout << s.size() << '\n';
  return 0;
}
