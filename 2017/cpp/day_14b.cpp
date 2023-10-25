#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <bitset>
#include <climits> 
#include <queue>
#include <utility>  

std::vector<int> split_input_into_lengths(const std::string& s) {
  std::vector<int> lengths;
  for (const auto c : s) {
    lengths.emplace_back(int(c));
  }
  lengths.emplace_back(17);
  lengths.emplace_back(31);
  lengths.emplace_back(73);
  lengths.emplace_back(47);
  lengths.emplace_back(23);
  return lengths;
}

void print(std::vector<int>& sparse_hash) {
  for (const auto& ele : sparse_hash) {
    std::cout << ele << ' ';
  }
  std::cout << '\n';
}

std::vector<int> create_hash_int(const std::string& line) {
  // std::cout << '|' << line << '|'<<'\n';
  constexpr int n = 256; // This will need to change to run with the sample input
  int current_position = 0;
  int prev_position = n - 1;
  int skip_size = 0;
  std::vector<int> sparse_hash(n);
  for (int i = 0; i < n; i++) {
    sparse_hash[i] = i; 
  }
  const auto lengths = split_input_into_lengths(line);
  for  (int round = 0; round < 64; round++) {
    for (const auto& length : lengths) {
      for (int i = 0; i < length/2; i++) {
        std::swap(sparse_hash[(current_position + i + n) % n], sparse_hash[(current_position + length - i + n - 1) % n]);
      }
      current_position += length + skip_size;
      current_position %= n;
      skip_size++;
      skip_size %= n;
    }
  }
  std::vector<int> dense_hash;
  for (int i = 0; i < n/ 16;i++) {
    dense_hash.emplace_back(sparse_hash[i*16]);
    for (int j = 1; j < 16; j++) {
      dense_hash[i] = dense_hash[i] ^ sparse_hash[i*16 + j]; 
    }
  }
  std::string result;
  std::bitset<128> result_bin;
  for (const auto& ele : dense_hash) {
    std::stringstream sstream;
    sstream << std::hex << ele;
    std::string temp = sstream.str();    
    result += (temp.size() < 2) ? "0" + temp : temp;
  }
  for (const auto c : result) {
    result_bin <<= 4;
    // std::cout << c << '\n';
    if (c >= '0' && c <= '9') {
      result_bin |= std::bitset<128>(c - '0');  
    } else {
      result_bin |= std::bitset<128>(c - 'a' + 10);  
    }
  }
  auto result_bin_str = result_bin.to_string();
  std::vector<int> result_int;
  for (auto& ele : result_bin_str) {
    if (ele == '1') {
      result_int.push_back(1);
    } else {
      result_int.push_back(0);
    }
  }
  return result_int;
}

void search(std::vector<std::vector<int>>& grid, const int row_in, const int col_in, const int region_count) {
  const std::vector<std::pair<int, int>> motions {
    {1,0},
    {0,1},
    {-1,0},
    {0,-1}
  };
  const int n_rows = grid.size();
  const int n_cols = grid[0].size();
  std::queue<std::pair<int, int>> points;
  points.emplace(row_in, col_in);
  if(grid[row_in][col_in] != 1) {
    std::cout << "This should not happen" << '\n';
    return;
  }
  while(!points.empty()) {
    const auto point = points.front();
    points.pop();
    // std::cout << "Current: " << point.first << ' ' << point.second << '\n';
    if(grid[point.first][point.second] != 1) {
      continue;
    }
    grid[point.first][point.second] = region_count;
    for (const auto& motion : motions) {
      const auto new_point = std::make_pair<int, int>(
        point.first + motion.first,
        point.second + motion.second
      );
      // std::cout << "New current: " << new_point.first << ' ' << new_point.second << '\n';
      if (new_point.first < 0 || new_point.second < 0 || new_point.first>= n_rows ||new_point.second >= n_cols) {
        continue;
      }
      if (grid[new_point.first][new_point.second] == 1) {
        points.push(new_point);
      }
    }
  }
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_14_input" ;  
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  std::vector<std::vector<int>> grid;
  for (int i = 0; i < 128; i++) {
    grid.push_back(create_hash_int(line + "-" + std::to_string(i)));
  }
  int region_count = 2;
  for (int row = 0; row < 128; row++) {
    for (int col = 0; col < 128; col++) {
      if (grid[row][col] == 1) {
        // std::cout << row << ' ' << col << '\n';
        region_count++;
        search(grid, row, col, region_count);
      }
    }
  }
  std::cout << region_count - 2 << '\n'; // Starting with 2 as 1,0 reresent used / free space
  return 0;
}