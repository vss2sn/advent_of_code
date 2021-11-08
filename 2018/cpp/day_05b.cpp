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
  const auto original_polymer = polymer;
  std::size_t smallest_size = original_polymer.size();
  for (char letter = 'a'; letter <= 'z'; letter++) {
    polymer = original_polymer;
    auto it = std::remove_if(std::begin(polymer), std::end(polymer),
      [letter, delta2](auto x){return x == letter || x == (letter + delta2); });
    polymer.erase(it, std::end(polymer));
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
    it = std::remove(std::begin(polymer), std::end(polymer), ' ');
    polymer.erase(it, std::end(polymer));
    const auto new_size = polymer.size();
    if (new_size < smallest_size) {
      smallest_size = new_size;
    }
  }
  std::cout << smallest_size << '\n';
  return smallest_size;
}
