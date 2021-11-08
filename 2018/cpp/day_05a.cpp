#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_05_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string polymer;
  std:getline(file, polymer);

  const size_t delta1 = int('a') - int('A');
  const size_t delta2 = int('A') - int('a');
  std::size_t start = 0;
  std::size_t end = 1;
  while(end < polymer.size()) {
    if (polymer[start] - polymer[end] == delta1 || polymer[start] - polymer[end] == delta2) {
      polymer[start] = ' ';
      polymer[end] = ' ';
      while (polymer[start] == ' ' && start > 0) {
        start--;
      }
    } else {
      start += 1;
    }
    while (polymer[start] == ' ') {
      start++;
    }
    end = start + 1;
    while (polymer[end] == ' ') {
      end++;
    }
  }
  auto it = std::remove(std::begin(polymer), std::end(polymer), ' ');
  polymer.erase(it, std::end(polymer));
  const auto ans = polymer.size();
  std::cout << ans << '\n';
  return ans;
}
