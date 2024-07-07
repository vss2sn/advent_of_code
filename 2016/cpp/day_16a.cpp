#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <string_view>
#include <vector>

std::vector<bool> process(const std::vector<bool>& a) {
    std::vector<bool> result = a;
    result.reserve(a.size() * 2 + 1);
    result.push_back(false);
    for (const auto ele : a | std::ranges::views::reverse) {
        result.push_back(!ele);
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  line = "10111100110001111";
  constexpr int n = 272; 
  std::vector<bool> a;
  std::vector<bool> b;
  a.reserve(line.size());
  for (const auto c : line) {
    a.push_back(c == '1' ? true : false);
  }
  // Fill disk
  while (a.size() < n) {
    a = process(a);
  }
  // Calculate checksum
  auto check_sum = std::vector<bool>(a.begin(), a.begin() + n);
  while (check_sum.size() % 2 == 0) {
    std::vector<bool> temp;
    temp.reserve(check_sum.size() / 2);
    for (int i = 0; i < check_sum.size(); i = i + 2) {
        if (check_sum[i] == check_sum[i+1]) {
            temp.push_back(true);
        } else {
            temp.push_back(false);
        }
    }
    check_sum = temp;
  }
  for (const auto ele : check_sum) {
    std::cout << (ele ? 1 : 0);
  }
  std::cout << '\n';
  return 0;
}