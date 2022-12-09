#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

// TODO: Improve hash function

void update_pair(const int k1, const int k2, std::array<std::array<int,2>, 10>& knots) {
  const auto delta = std::array<int,2>{knots[k1][0] - knots[k2][0], knots[k1][1] - knots[k2][1]};
  if (std::abs(delta[0]) == 2 && std::abs(delta[1]) == 0) {
    knots[k2][0] += delta[0]/2;
  } else if (std::abs(delta[0]) == 2 && std::abs(delta[1]) == 1) {
    knots[k2][0] += delta[0]/2;
    knots[k2][1] += delta[1];
  } else if (std::abs(delta[0]) == 0 && std::abs(delta[1]) == 2) {
    knots[k2][1] += delta[1]/2;
  } else if (std::abs(delta[0]) == 1 && std::abs(delta[1]) == 2) {
    knots[k2][0] += delta[0];
    knots[k2][1] += delta[1]/2;
  } else if (std::abs(delta[0]) == 2 && std::abs(delta[1]) == 2) {
    knots[k2][0] += delta[0]/2;
    knots[k2][1] += delta[1]/2;
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::array<std::array<int, 2>, 10> knots;
  for (auto& row : knots) {
    for (auto& ele : row) {
      ele = 0;
    }
  }

  std::unordered_set<int> s;
  while(std::getline(file, line)) {
    for (int i = 0; i < std::stoi(line.substr(2, line.size() - 2)); i++) {
      if (line[0] == 'U') {
        knots[0][0] += 1;
      } else if (line[0] == 'D') {
        knots[0][0] -= 1;
      } else if (line[0] == 'R') {
        knots[0][1] += 1;
      } else {
        knots[0][1] -= 1;
      }
      for (int i = 0; i < knots.size() - 1; i++) {
        update_pair(i, i+1, knots);
      }
      s.insert(knots[9][0] * 100000 + knots[9][1]);
    }
  }

  std::cout << s.size() << '\n';
  return 0;
}
