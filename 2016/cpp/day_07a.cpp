#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ranges>
#include <string_view>
#include <algorithm>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  int ports_with_tls_support = 0;
  while(std::getline(file, line)) {
    if (line.empty()) continue;
    int bracket_count = 0;
    bool supports = false;
    if (line[0] == '[') bracket_count++;
    for (std::size_t idx = 1; idx < line.size() - 2; idx++) {
        if (line[idx] == '[') {
            bracket_count++;
            // supports = false;
        } else if (line[idx] == ']') {
            bracket_count--;
            // supports = false;
        } else if (line[idx] == line[idx+1] 
                    && line[idx-1] == line[idx+2] 
                    && line[idx-1] != line[idx] 
                    && std::all_of(
                        std::next(std::begin(line), idx - 1), 
                        std::next(std::begin(line), idx + 2), 
                        [](const auto c) {
                            return c != '[' && c !=']';
                        })){
            if (bracket_count != 0) {
                supports = false;
                break;
            } else {
                // std::cout << line[idx-1] << line[idx]<<line[idx+1]<<line[idx+2]<< '\n';
                supports = true;
            }
        }
    }
    if (supports) {
            // std::cout << line << '\n';
            ports_with_tls_support++;
        }
  }
  std::cout << ports_with_tls_support << '\n';
  return 0;
}