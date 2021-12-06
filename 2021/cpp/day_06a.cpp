#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print(const std::vector<int>& v) {
  for (const auto ele : v) {
    std::cout << ele << ',';
  }
  std::cout << '\n';
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  std::vector<int> fish;
  std::size_t start = 0;
  std::size_t end = line.find(',', start);
  while (end != std::string::npos) {
    fish.emplace_back(std::stoi(line.substr(start, end - start)));
    start = end + 1;
    end = line.find(',', start);
  }
  fish.emplace_back(std::stoi(line.substr(start, line.size() - start)));
  // print(fish);
  for (int day = 0; day < 80; day++) {
    int to_add = 0;
    for (auto& f : fish) {
      if (f == 0) {
          to_add++;
          f = 7;
      }
      f--;
    }
    for (int i = 0; i < to_add; i++) {
      fish.emplace_back(8);
    }
    // print(fish);
  }
  std::cout << fish.size() << '\n';
  return 0;
}
