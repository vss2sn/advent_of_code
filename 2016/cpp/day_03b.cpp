#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <string_view>
#include <vector>

const auto join_character_in_each_subranges = [](auto &&rng) { return std::string_view(&*rng.begin(), std::ranges::distance(rng)); };

int convert_to_int(const std::string_view s) {
    int num = 0;
    for (const auto c : s) {
        num = num * 10 + (c - '0');
    }
    return num;
}

bool check_triangle(const int n0, const int n1, const int n2) {
    if (n0 + n2 <= n1) return false;
    if (n1 + n0 <= n2) return false;
    if (n2 + n1 <= n0) return false;
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
  std::vector<std::vector<int>> numbers;
  while(std::getline(file, line)) {
    numbers.emplace_back();
    for (const auto num : line 
                            | std::ranges::views::split(' ') 
                            | std::ranges::views::transform(join_character_in_each_subranges))
    {
        if (num.empty()) continue;
        numbers.back().emplace_back(convert_to_int(num));
    }
  }
  for (std::size_t row_idx = 0; row_idx < numbers.size(); row_idx = row_idx + 3) {
    for (std::size_t col_idx = 0; col_idx < 3; col_idx++) {
        if(check_triangle(numbers[row_idx][col_idx], numbers[row_idx+1][col_idx], numbers[row_idx+2][col_idx])) count++;
    }
  }
  
  std::cout << count << '\n';
  return 0;
}