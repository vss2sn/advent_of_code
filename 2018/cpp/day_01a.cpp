#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  constexpr int start_f = 0;
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  long long sum = 0;
  std::ifstream file(input);
  std::string num;
  while(std::getline(file, num)) {
    if (num.substr(0, 1) == "+") {
      sum += stoll(num.substr(1, num.size() - 1));
    } else {
      sum += stoll(num);
    }
  }
  std::cout << sum << '\n';
  return sum;
}
