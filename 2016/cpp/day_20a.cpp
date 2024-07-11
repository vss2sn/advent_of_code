#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>  

struct Range {
    long long start;
    long long end;
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_20_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::vector<Range> ranges;
  while(std::getline(file, line)) {
    const auto dash_idx = line.find('-');
    Range range;
    std::cout << '|' << line.substr(0, dash_idx) << '|' << '\n';
    std::cout << '|' << line.substr(dash_idx + 1, line.size() - dash_idx - 1) << '|' << '\n';
    range.start = std::stoll(line.substr(0, dash_idx));
    range.end = std::stoll(line.substr(dash_idx + 1, line.size() - dash_idx - 1));
    ranges.push_back(range);
  }
  std::sort(std::begin(ranges), std::end(ranges), [](const Range& r1, const Range& r2) { return r1.start < r2.start; });
  if (ranges[0].start != 0) {
    std::cout << 0 << '\n';
    return 0;
  }
  for (std::size_t idx = 0; idx < ranges.size() - 1; idx++) {
    if (ranges[idx].end + 1  < ranges[idx+1].start) {
        std::cout << ranges[idx].end + 1 << '\n';
        return 0;
    }
  }
  std::cout << ranges.back().end+1 << '\n';
  return 0;
}