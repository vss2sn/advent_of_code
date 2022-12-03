#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

// Can use the same method as 3a, but with an & for the 2nd comparison instead of |

int main(int argc, char * argv[]) {
  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  int sum = 0;
  std::array<int, 58> counter;
  std::fill(std::begin(counter), std::end(counter), 0);
  while(std::getline(file, line)) {
    // Account for CRLF if required
    // line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    std::sort(std::begin(line), std::end(line));
    line.erase(std::unique(std::begin(line), std::end(line)), std::end(line));
    for (const auto c : line) {
      counter[c - 'A'] += 1;
      if (counter[c - 'A'] == 3) {
        if (c >= 'a') {
          sum += c - 'a' + 1;
        } else {
          sum += c - 'A' + 1 + 26;
        }
        std::fill(std::begin(counter), std::end(counter), 0);
        break;
      }
    }
  }
  std::cout << sum << '\n';
  return 0;
}
