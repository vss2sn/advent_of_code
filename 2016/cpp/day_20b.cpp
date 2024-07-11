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
  long long allowed = 0;
  while(std::getline(file, line)) {
    const auto dash_idx = line.find('-');
    Range range;
    range.start = std::stoll(line.substr(0, dash_idx));
    range.end = std::stoll(line.substr(dash_idx + 1, line.size() - dash_idx - 1));
    ranges.push_back(range);
  }
  std::sort(std::begin(ranges), std::end(ranges), [](const Range& r1, const Range& r2) { return r1.start < r2.start; });

  int range_idx = 0;
  for (std::size_t idx = 0; idx < ranges.size() - 1; idx++) {
    if (ranges[range_idx].end >= ranges[idx+1].start) {
      Range range;
      range.start = ranges[range_idx].start;
      range.end = (ranges[range_idx].end > ranges[idx + 1].end) ? 
                    ranges[range_idx].end : 
                    ranges[idx + 1].end;
      ranges[range_idx] = range;
    } else {
      range_idx = idx+1;
    }
  }
  std::vector<Range> combined_ranges{ranges[0]};
  range_idx = 0;
  for (std::size_t idx = 1; idx < ranges.size(); idx++) {
    if (ranges[idx].start > ranges[range_idx].end) {
      range_idx = idx;
      combined_ranges.push_back(ranges[idx]);
    } 
  }

  std::swap(ranges, combined_ranges);
  if (ranges[0].start != 0) {
    allowed += ranges[0].start;
  }
  for (std::size_t idx = 0; idx < ranges.size() - 1; idx++) {
    if (ranges[idx].end + 1  < ranges[idx+1].start) {
        allowed += ranges[idx + 1].start - ranges[idx].end - 1;
    }
  }
  std::cout << allowed << '\n';
  return 0;
}