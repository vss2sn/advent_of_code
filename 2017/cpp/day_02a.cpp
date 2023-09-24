#include <fstream>
#include <iostream>
#include <string>
#include <charconv>
#include <limits>
#include <ranges>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_02_input" ;  
  std::ifstream file(input);
  std::string line;
  int checksum = 0; 
  while(std::getline(file, line)) {
    auto numbers = line 
                    | std::ranges::views::split(' ') 
                    | std::ranges::views::transform([](auto&& rng) { 
                        return std::string_view(&*rng.begin(), std::ranges::distance(rng)); });
    int min_val = std::numeric_limits<int>::max();
    int max_val = std::numeric_limits<int>::min();
    for (const auto n : numbers) {
        int val;
        std::from_chars(n.data(), n.data() + n.size(), val);
        max_val = std::max(val, max_val);
        min_val = std::min(val, min_val);
    }
    checksum += max_val - min_val;
  }
  std::cout << checksum << '\n';
  return 0;
}
