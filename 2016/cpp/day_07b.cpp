#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <unordered_set>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  int ports_with_ssl_support = 0;
  while(std::getline(file, line)) {
    if (line.empty()) continue;
    int bracket_count = 0;
    // bool supports = false;
    std::unordered_set<std::string_view> in_brackets;
    std::unordered_set<std::string_view> out_of_brackets;
    // if (line[0] == '[') bracket_count++;
    for (std::size_t idx = 0; idx < line.size() - 2; idx++) {
        if (line[idx] == '[') {
            bracket_count++;
            // supports = false;
        } else if (line[idx] == ']') {
            bracket_count--;
            // supports = false;
        } else if (line[idx] == line[idx+2] 
                    && line[idx] != line[idx+1] 
                    && line[idx+1] != '['
                    && line[idx+1] != ']'
                    // not required as starting with idx
                    // && std::all_of(std::next(std::begin(line), idx), std::next(std::begin(line), idx + 2), 
                    //     [](const auto c) { return c != '[' && c !=']'; })
                ){
            if (bracket_count != 0) {
                in_brackets.insert(std::string_view(std::next(line.begin(), idx), std::next(line.begin(), idx) + 3));
                // std::cout << "in: " << std::string_view(std::next(line.begin(), idx), std::next(line.begin(), idx) + 3) << '\n';
            } else {
                out_of_brackets.insert(std::string_view(std::next(line.begin(), idx), std::next(line.begin(), idx) + 3));
                // std::cout << "out: " << std::string_view(std::next(line.begin(), idx), std::next(line.begin(), idx) + 3) << '\n';
            }
        }
    }
    // std::cout << "here"<< '\n';
    for (const auto s_in : in_brackets) {
        // std::cout << "s_in: " << s_in << '\n';
        // std::cout << "" << s_in[1] << s_in[0] <<  s_in[1] << '\n';
        const auto s_corr = std::string() + s_in[1] + s_in[0]+ s_in[1];
        // std::cout << "s_corr: " << s_corr << '\n';
        if (out_of_brackets.find(std::string_view(s_corr)) != out_of_brackets.end()) {
            ports_with_ssl_support++;
            break;
        }
    } 
  }
  std::cout << ports_with_ssl_support << '\n';
  return 0;
}