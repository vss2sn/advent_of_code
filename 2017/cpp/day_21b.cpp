#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cassert>   
#include <cmath>

// TODO: refactor this.
// Can use a vector instead of a map since the final size is known from before, then no need to run the hasher constantly
// There is probably a better way to predict the final pattern or jump steps, 
// for example, cache patterns seen and what they transform into n iterations later
// TODO: Investigate why running this at O3 causes a crash while O0 is fine
struct Coord2D {
  int row;
  int col;

  Coord2D(const int row = 0, const int col = 0) : row(row) , col(col) {}

  Coord2D operator + (const Coord2D& c) const {
    Coord2D ans;
    ans.row = row + c.row;
    ans.col = col + c.col;
    return ans;
  }

bool operator == (const Coord2D& c) const {
  return row == c.row && col == c.col;
}



  Coord2D operator - (const Coord2D& c) const {
    Coord2D ans;
    ans.row = row - c.row;
    ans.col = col - c.col;
    return ans;
  }

  Coord2D operator += (const Coord2D& c) {
    row += c.row;
    col += c.col;
    return *this;
  }
};

struct hasher {
  std::size_t operator()(const Coord2D& c) const {
    return 1000000 * c.row + c.col;
  }
};

void print_pattern(const std::unordered_map<Coord2D, char, hasher>& pattern) {
  const int dim = std::sqrt(pattern.size());
  for (int row = 0; row < dim; row++) {
      for (int col = 0; col < dim; col++) {
        std::cout << pattern.at(Coord2D(row,col));
      }
      std::cout << '\n';
    }
    std::cout << '\n';
}

int count_on_in_pattern(const std::unordered_map<Coord2D, char, hasher>& pattern) {
  const int dim = std::sqrt(pattern.size());
  int count = 0;
  for (int row = 0; row < dim; row++) {
      for (int col = 0; col < dim; col++) {
        if(pattern.at(Coord2D(row,col)) == '#') count++;
      }
    }
  return count;
}


std::string pattern_to_string(const std::unordered_map<Coord2D, char, hasher>& map) {
  std::string ans = "";
  const int n = (map.size() == 4) ? 2 : 3;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      ans += map.at(Coord2D(row, col));
    }
    if (row != n - 1) ans += '/';
  }
  return ans;
}

std::unordered_map<Coord2D, char, hasher> string_to_pattern(const std::string& s) {
  std::unordered_map<Coord2D, char, hasher> map;
  int n = 0;
  if(s.size() == 5) {
    n = 2;
  } else if(s.size() == 11) {
    n = 3;
  } else if(s.size() == 19) {
    n = 4;
  }
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      map[Coord2D(row, col)] = s[row * (n + 1) + col]; // n+1 to include the '/'
    }
  }
  return map;
}

std::unordered_map<Coord2D, char, hasher> rotate_pattern(const std::unordered_map<Coord2D, char, hasher>& map) {
  std::unordered_map<Coord2D, char, hasher> ans;
  const int n = map.size() == 4 ? 2 :3;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      ans[Coord2D(row, col)] = map.at(Coord2D(n - col - 1, row));
      // std::cout << ans[Coord2D(row, col)];
    }
    // std::cout << '\n';
  }
  // std::cout << '\n';
  return ans;
}

std::unordered_map<Coord2D, char, hasher> flip_pattern(const std::unordered_map<Coord2D, char, hasher>& map) {
  std::unordered_map<Coord2D, char, hasher> ans;
  const int n = map.size() == 4 ? 2 :3;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      ans[Coord2D(n - row - 1, col)] = map.at(Coord2D(row, col));
    }
  }
  return ans;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_21_input" ;  
  std::ifstream file(input);
  std::string line;
  const std::regex regex_pattern(R"(([#./]+) => ([#./]+))");
  std::unordered_map<std::string, std::unordered_map<Coord2D, char, hasher>> rules;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, regex_pattern);
    const std::string input_str = match[1];
    // std::cout << input_str << '\n';
    // std::cout << pattern_to_string(string_to_pattern(input_str)) << '\n';
    assert(input_str == pattern_to_string(string_to_pattern(input_str)));
    const auto output_pattern_str = match[2];
    // std::cout << output_pattern_str << '\n';
    const auto output_pattern = string_to_pattern(output_pattern_str);
    auto pattern = string_to_pattern(input_str);
    
    // rules[pattern_to_string(pattern)] = output_pattern;
    // rules[pattern_to_string(flip_pattern(pattern))] = output_pattern;    
    for (int i = 0; i < 4; i++) {
      pattern = rotate_pattern(pattern);
      rules[pattern_to_string(pattern)] = output_pattern;
      rules[pattern_to_string(flip_pattern(pattern))] = output_pattern;
    }
  }

  // .#.
  // ..#
  // ###
  std::unordered_map<Coord2D, char, hasher> pattern;
  std::unordered_map<Coord2D, char, hasher> new_pattern;
  pattern[Coord2D(0,0)] = '.';
  pattern[Coord2D(0,1)] = '#';
  pattern[Coord2D(0,2)] = '.';
  pattern[Coord2D(1,0)] = '.';
  pattern[Coord2D(1,1)] = '.';
  pattern[Coord2D(1,2)] = '#';
  pattern[Coord2D(2,0)] = '#';
  pattern[Coord2D(2,1)] = '#';
  pattern[Coord2D(2,2)] = '#';

  int n_parts = 0;
  int n = 0;
  int iterations = 0;
  // Fix (0,0)
  for (int iterations = 0; iterations < 18; iterations++) {
    // std::cout << "Iteration: " << iterations << '\n';
    const int dim = std::sqrt(pattern.size());

    if (dim % 2 == 0) {
      n_parts = dim/2;
      n = 2; 
    } else if (dim % 3 == 0) {
      n_parts = dim/3;
      n = 3;
    } else {
      std::cout << "Weird" << '\n';
      std::cout << dim <<  '\n';
      exit(0);
    }
    // std::cout << "dim == " << dim << '\n';
    // std::cout << "n_parts == " << n_parts << '\n';
    // std::cout << "n == " << n << '\n';
    // std::cout << "Pattern: " <<'\n';
    // print_pattern(pattern);
    for (int parts_row = 0; parts_row < n_parts; parts_row++) {
      const int row = n * parts_row;
      for (int parts_col = 0; parts_col < n_parts; parts_col++) {
        const int col = n * parts_col;
        std::unordered_map<Coord2D, char, hasher> mini_pattern;
        for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
            mini_pattern[Coord2D(i,j)]= pattern[Coord2D(row + i, col + j)];
          }
        }
        // std::cout << "Mini pattern: " <<'\n';
        // print_pattern(mini_pattern);
        const auto new_mini_pattern = rules.at(pattern_to_string(mini_pattern));
        // std::cout << "New mini pattern: " <<'\n';
        // print_pattern(new_mini_pattern);
        for (int i = 0; i < n + 1; i++) {
          for (int j = 0; j < n + 1; j++) {
            new_pattern[Coord2D((n + 1)* parts_row + i, (n + 1) * parts_col + j)] = new_mini_pattern.at(Coord2D(i,j));
          }
        }
      }
    }
    std::swap(pattern, new_pattern);
    // std::cout << count_on_in_pattern(pattern) <<'\n';

  }
  // std::cout << "Pattern: " <<'\n';
  // print_pattern(pattern);
  std::cout << count_on_in_pattern(pattern) <<'\n';
  return 0;
}