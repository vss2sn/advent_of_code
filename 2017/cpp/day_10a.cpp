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
  std::size_t start = 0;
  const std::string delimiter = ",";
  std::size_t end = s.find(delimiter);
  while(end != std::string::npos) {
    lengths.emplace_back(std::stoi(s.substr(start, end - start)));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
  }
  lengths.emplace_back(std::stoi(s.substr(start, end - start)));
  return lengths;
}

void print(std::vector<int>& hash_str) {
  for (const auto& ele : hash_str) {
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
  std::vector<int> hash_str(n);
  for (int i = 0; i < n; i++) {
    hash_str[i] = i; 
  }
  // print(hash_str);
  const auto lengths = split_input_into_lengths(line);
  for (const auto& length : lengths) {

    // std::cout << "length: " << length << '\n';
    for (int i = 0; i < length/2; i++) {
      // std::cout << "swapping positions: " << (current_position + i + n) % n << " and " << (current_position + length - i + n - 1) % n << '\n';
      // std::cout << "swapping values: " << hash_str[(current_position + i + n) % n] << " and " << hash_str[(current_position + length - i + n - 1) % n] << '\n';
      std::swap(hash_str[(current_position + i + n) % n], hash_str[(current_position + length - i + n - 1) % n]);
    }
    // print(hash_str);
    current_position += length + skip_size;
    skip_size++;
  }
  std::cout << hash_str[0] * hash_str[1] << '\n';
  // std::cout << hash_str[current_position % n] * hash_str[(current_position + 1) % n] << '\n';
  return 0;
}