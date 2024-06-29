#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <unordered_set>

struct Marker {
  std::size_t start_idx = 0;
  std::size_t end_idx = 0;
  int n_chars = 0;
  int n_repeats = 0;
};

std::vector<Marker> parse (const std::string& line) {
  std::vector<Marker> markers;
  for (std::size_t i = 0; i < line.size(); i++) {
    if (line[i] == '(') {
      markers.emplace_back();
      auto& m = markers.back();
      m.start_idx = i;
      const auto x_idx = line.find('x', m.start_idx);
      m.end_idx = line.find(')', m.start_idx);
      // std::cout << '|' << line.substr(m.start_idx+1, x_idx - m.start_idx - 1) <<  '|' << '\n';
      // std::cout << '|' <<  line.substr(x_idx + 1, m.end_idx - x_idx - 1) <<  '|' << '\n';
      m.n_chars = std::stoi(line.substr(m.start_idx  + 1, x_idx - m.start_idx - 1));
      m.n_repeats = std::stoi(line.substr(x_idx + 1, m.end_idx - x_idx - 1));
    //   std::cout << m.start_idx << ' ' << m.end_idx <<  ' '<< m.n_chars << ' ' << m.n_repeats << '\n';
    }
  }
  return markers; 
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  for (const auto ele : std::views::istream<std::string>(file)) {
    std::cout << ele << '\n';
  }
  while(std::getline(file, line)) {
    // std::cout << line << "\n";
    const auto markers = parse(line);
    std::vector<std::size_t> multipliers(line.size(), 1);
    std::size_t idx = 0;
    for (const auto& marker : markers) {
    //   if (marker.start_idx < idx) continue;
      idx = marker.start_idx;
      while (idx <=  marker.end_idx) {
        multipliers[idx] = 0;
        // std::cout <<  '0';
        idx++;
      }
      for (idx = marker.end_idx + 1; idx <= marker.end_idx + marker.n_chars; idx++) {
        // std::cout << marker.n_repeats << '\n';
        multipliers[idx] *= marker.n_repeats;
        // std::cout << multipliers[idx] << '\n';
      }
    }
    // for (const auto m : multipliers) {
    //   std::cout << m;
    // }
    // std::cout << '\n';
    const auto total = std::accumulate(std::begin(multipliers), std::end(multipliers), std::size_t(0));
    std::cout << total << '\n';
  }
  return 0;
}