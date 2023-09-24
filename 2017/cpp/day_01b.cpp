#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  long long sum = 0;
  std::ifstream file(input);
  std::string num;
  std::getline(file, num);
  std::size_t count = 0;
  const auto n = num.size() / 2;
  for(std::size_t idx = 0; idx < n; idx++) {
    if (num[idx] == num[idx + n]) {
        count += num[idx] - '0';
    }
  }
  count *= 2;
  std::cout << count << '\n';
  return count;
}
