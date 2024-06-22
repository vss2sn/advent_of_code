#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <string_view>
#include <vector>

auto join_character_in_each_subranges = [](auto &&rng) { return std::string_view(&*rng.begin(), std::ranges::distance(rng)); };

int convert_to_int(const std::string_view s) {
    int num = 0;
    for (const auto c : s) {
        num = num * 10 + (c - '0');
    }
    return num;
}

bool check_triangle(const std::vector<int>& numbers) {
    if (numbers[0] + numbers[1] <= numbers[2]) return false;
    if (numbers[1] + numbers[2] <= numbers[0]) return false;
    if (numbers[2] + numbers[0] <= numbers[1]) return false;
    return true;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  int count = 0;
  while(std::getline(file, line)) {
    std::vector<int> numbers;
    numbers.reserve(3);
    for (const auto num : line 
                            | std::ranges::views::split(' ') 
                            | std::ranges::views::transform(join_character_in_each_subranges))
    {
        if (num.empty()) continue;
        numbers.push_back(convert_to_int(num));
        std::cout << numbers.back() << ",";
    }
    std::cout << '\n';
    if(check_triangle(numbers)) count++;

  }
  std::cout << count << '\n';
  return 0;
}