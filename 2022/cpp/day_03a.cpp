#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

// While method 1 is faster than method 2 in theory x/(log(x) + 2) > 4
// creating the 2 new strings and inserting at the back of the vector
// seem to remove any advantage in practise
// Method 1
// std::vector<char> intersection;
// auto s1 = line.substr(0, line.size()/2);
// auto s2 = line.substr(line.size()/2,line.size()/2);
// std::sort(std::begin(s1), std::end(s1));
// std::sort(std::begin(s2), std::end(s2));
// std::set_intersection(std::begin(s1), std::end(s1), std::begin(s2), std::end(s2), std::back_inserter(intersection));
// const auto c = intersection[0];
// Method 2
// const auto c = *std::find_first_of(std::begin(line), std::next(std::begin(line), line.size()/2),
//                                   std::next(std::begin(line), line.size()/2), std::end(line));
// Method 3 - implemented below

int main(int argc, char * argv[]) {
  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  int sum = 0;
  while(std::getline(file, line)) {
    // Account for CRLF if required
    // line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    uint64_t p1 = 0;
    uint64_t p2 = 0;
    for (int i = 0; i < line.size() / 2; i++) {
      p1 |= 1UL << (line[i] - 65UL);  // 65 == 'A'
      p2 |= 1UL << (line[i + line.size() / 2] - 65UL);
    }

    // const char c = 65 + std::log2(p1 & p2);
    if (const char c = 65 + 63 - __builtin_clzll(p1 & p2); c >= 'a') {
      sum += c - 'a' + 1;
    } else {
      sum += c - 'A' + 1 + 26;
    }
  }
  std::cout << sum << '\n';
  return 0;
}
