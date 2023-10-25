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

std::string create_hash(const std::string& line) {
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
    
  return result_bin.to_string();
}


int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_14_input" ;  
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  int used_bits = 0;
  for (int i = 0; i < 128; i++) {
    const auto grid_row = create_hash(line + "-" + std::to_string(i));
    used_bits += std::count_if(std::begin(grid_row), std::end(grid_row), [](const auto ele) {return ele == '1';});
  }
  std::cout << used_bits << '\n';
  return 0;
}