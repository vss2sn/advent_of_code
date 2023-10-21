#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>

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

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_10_input" ;  
  std::ifstream file(input);
  std::string line;
  constexpr int n = 256; // This will need to change to run with the sample input
  
  std::getline(file, line);
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
  // print(sparse_hash);    
  std::vector<int> dense_hash;
  for (int i = 0; i < n/ 16;i++) {
    dense_hash.emplace_back(sparse_hash[i*16]);
    for (int j = 1; j < 16; j++) {
      dense_hash[i] = dense_hash[i] ^ sparse_hash[i*16 + j]; 
    }
  }
  std::string result;
  for (const auto& ele : dense_hash) {
    std::stringstream sstream;
    sstream << std::hex << ele;
    std::string temp = sstream.str();
    result += (temp.size() < 2) ? "0" + temp : temp;
  }
  std::cout << result << '\n';
  return 0;
}