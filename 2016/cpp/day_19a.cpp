#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  const int n_elves = std::stoi(line);
  std::unordered_map<int, int> elves;
  for (int i = 1; i <= n_elves;i++) {
    elves[i] = i+1;
  }
  elves[n_elves] = 1;
  int current = 1;
  while(elves[current] != current) {
    // std::cout << elves[current] << " out" << '\n';
    elves[current] = elves[elves[current]];
    current = elves[current];
  }
  std::cout << current << '\n';
  return 0;
}