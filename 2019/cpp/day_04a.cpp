#include <fstream>
#include <iostream>
#include <string>

bool meetsConditions(int num) {
  int prev_val = 10;
  bool repeat = false;
  while (num > 0) {
    const auto val = num % 10;
    num = int(num / 10);
    if (val > prev_val) {
      return false;
    }
    if (prev_val == val) {
      repeat = true;
    }
    prev_val = val;
  }
  return repeat;
}

int main(int argc, char *argv[]) {
  // Get input
  std::string input = "../input/day_04_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_str;
  std::getline(file, input_str);

  const auto delimiter_index = input_str.find("-");
  auto min_val = std::stoi(input_str.substr(0, delimiter_index));
  auto max_val = std::stoi(input_str.substr(
      delimiter_index + 1, input_str.size() - delimiter_index - 1));

  // Solve
  size_t count = 0;
  for (int num = min_val; num < max_val; ++num) {
    if (meetsConditions(num)) {
      count += 1;
    }
  }

  std::cout << count << '\n';
  return count;
}
