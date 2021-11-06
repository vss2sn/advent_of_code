#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[]) {
  constexpr int start_f = 0;
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  long long sum = 0;
  std::unordered_set<long long> seen_vals;
  seen_vals.insert(start_f);
  bool duplicate_found = false;

  while (!duplicate_found) {
    std::ifstream file(input);
    std::string num;
    while(std::getline(file, num)) {
      if (num.substr(0, 1) == "+") {
        sum += stoll(num.substr(1, num.size() - 1));
      } else {
        sum += stoll(num);
      }
      if (const auto [iter, inserted] = seen_vals.insert(sum); !inserted) {
        duplicate_found = true;
        break;
      }
    }
  }
  std::cout << sum << '\n';
  return sum;
}
