#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<int> totals(1, 0);
  while(std::getline(file, line)) {
    // Account for CRLF if required
    // line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line.size() == 0) {
      totals.emplace_back(0);
    } else {
      totals.back() += std::stoi(line);
    }
  }

  // Can use sort, or partial-sort as well,
  // but since first 3 largest numbers in any order, nth_element works
  std::nth_element(std::begin(totals), std::begin(totals) + 3, std::end(totals), std::greater<int>());
  const auto sum = totals[0] + totals[1] + totals[2];
  std::cout << sum << '\n';
  return sum;
}
