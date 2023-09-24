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
  for(std::size_t idx = 0; idx < num.size()-1; idx++) {
    if (num[idx] == num[idx+1]) {
        count += num[idx] - '0';
    }
  }
  if (num.size() > 0 && num[num.size() - 1] == num[0]) {
    count += num[0] - '0';
  }
  std::cout << count << '\n';
  return count;
}
