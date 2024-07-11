#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>  
#include <unordered_map>  

int sv_to_int (const std::string_view sv) {
  bool negative = (sv[0] == '-') ? true : false;
  int n = 0;
  for (int i = negative ? 1 : 0; i < sv.size(); i++) {
    n = n * 10 + (sv[i] - '0');
  }
  if (negative) {
    n *= -1;
  }
  return n;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_21_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::unordered_map<char, int> char_to_idx;
  std::unordered_map<int, char> idx_to_char;
  
  std::vector<std::vector<std::string>> instructions;
  std::string p = "abcdefgh";
  std::string result = "00000000";
  const int n = p.size();

  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    std::vector<std::string> split_line;
    std::ranges::copy(line 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string{std::string_view(rng)};}), 
                            std::back_inserter(split_line));
    instructions.push_back(split_line);
  }
  // Assumes a valid permutation will be found
  while(true){
    // std::cout << p << '\n';
    for (int i = 0; i <= p.size(); i++) {
      char_to_idx[p[i]] = i;
      idx_to_char[i] = p[i];
    }
    for (const auto& split_line : instructions) {
      if (split_line[0] == "swap" && split_line[1] == "position") {
        const int idx_1 = sv_to_int(split_line[2]);
        const int idx_2 = sv_to_int(split_line[5]);
        const char c1 = idx_to_char[idx_1];
        const char c2 = idx_to_char[idx_2];
        idx_to_char[idx_1] = c2;
        idx_to_char[idx_2] = c1;
        char_to_idx[c1] = idx_2;
        char_to_idx[c2] = idx_1;
      }
      else if (split_line[0] == "swap" && split_line[1] == "letter") {
        const char c1 = split_line[2][0];
        const char c2 = split_line[5][0];
        const int idx_1 = char_to_idx[c1];
        const int idx_2 = char_to_idx[c2];
        idx_to_char[idx_1] = c2;
        idx_to_char[idx_2] = c1;
        char_to_idx[c1] = idx_2;
        char_to_idx[c2] = idx_1;
      }
      else if (split_line[0] == "rotate" && split_line[1] == "left") {
        const int rotate_n = (-sv_to_int(split_line[2]) + n) % n;
        for (auto& [c, idx] : char_to_idx) {
            idx = (idx + rotate_n) % n;
            idx_to_char[idx] = c;
        }
      }
      else if (split_line[0] == "rotate" && split_line[1] == "right") {
        const int rotate_n = sv_to_int(split_line[2]) % n;
        for (auto& [c, idx] : char_to_idx) {
            idx = (idx + rotate_n) % n;
            idx_to_char[idx] = c;
        }
      }
      else if (split_line[0] == "rotate" && split_line[1] == "based") {
        const auto c_idx = char_to_idx[split_line[6][0]];
        const int rotate_n = 1 + c_idx + ((c_idx >= 4) ? 1 : 0);
        for (auto& [c, idx] : char_to_idx) {
            idx = (idx + rotate_n + n) % n;
            idx_to_char[idx] = c;
        }
      }
      else if (split_line[0] == "reverse") {
        const int idx_1 = sv_to_int(split_line[2]);
        const int idx_2 = sv_to_int(split_line[4]);
        for (auto& [c, idx] : char_to_idx) {
            if (idx >= idx_1 && idx <= idx_2) {
                idx = idx_2 - idx + idx_1;
                idx_to_char[idx] = c;
            }
        }
      }
      else if (split_line[0] == "move" && split_line[1] == "position") {
        const int idx_1 = sv_to_int(split_line[2]);
        const int idx_2 = sv_to_int(split_line[5]);
        char c_prime = idx_to_char[idx_1];
        if (idx_2 > idx_1) {
            for (auto& [c, idx] : char_to_idx) {
                if (idx > idx_1 && idx <= idx_2) {
                    idx--;
                    idx_to_char[idx] = c;
                }
            }
            char_to_idx[c_prime] = idx_2;
            idx_to_char[idx_2] = c_prime;
        } else if (idx_1 > idx_2) {
            for (auto& [c, idx] : char_to_idx) {
                if (idx >= idx_2 && idx < idx_1) {
                    idx++;
                    idx_to_char[idx] = c;
                }
            }
            char_to_idx[c_prime] = idx_2;
            idx_to_char[idx_2] = c_prime;
        }
      }
    }
    for (const auto [c, idx] : char_to_idx) {
        result[idx] = c;
    }
    if (result == "fbgdceah") {
      std::cout << p << '\n';
      return 0;
    }
    std::next_permutation(p.begin(), p.end());
  }
  std::cout << "No valid permutation found" << '\n';
  return 0;
}
