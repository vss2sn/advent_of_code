#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  int count = 0;
  const auto unique_size = [](const size_t val) { return val == 2 || val == 4 || val == 3 || val == 7; };
  while(std::getline(file, line)) {
    std::size_t start = 0;
    std::string delimiter = " ";
    std::size_t end = line.find(delimiter);
    std::vector<std::vector<std::string>> lhs;
    std::vector<std::vector<std::string>> rhs;
    bool val_lhs = true;
    lhs.emplace_back();
    rhs.emplace_back();
    while (end != std::string::npos) {
      auto temp = line.substr(start, end - start);
      std::sort(std::begin(temp), std::end(temp));
      if (temp == "|") {
        val_lhs = false;
      }
      else if (val_lhs) {
        lhs.back().emplace_back(temp);
      } else {
        rhs.back().emplace_back(temp);
        if (unique_size(temp.size())) count++;
      }
      start = end + 1;
      end = line.find(delimiter, start);
    }
    auto temp = line.substr(start, line.size() - start);
    std::sort(std::begin(temp), std::end(temp));
    rhs.back().emplace_back(temp);
    if (unique_size(temp.size())) count++;
  }
  std::cout << count << '\n';
  return 0;
}
