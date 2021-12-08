#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


int count_common_elements(const std::string& s1, const std::string& s2) {
  int count = 0;
  for (const auto ele : s1) {
    if (s2.find(ele) != std::string::npos) count++;
  }
  return count;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  const auto unique_size = [](const size_t val) { return val == 2 || val == 4 || val == 3 || val == 7; };
  std::vector<std::vector<std::string>> total;
  std::vector<std::vector<std::string>> rhs;
  while(std::getline(file, line)) {
    std::size_t start = 0;
    std::string delimiter = " ";
    std::size_t end = line.find(delimiter);
    bool val_lhs = true;
    total.emplace_back();
    rhs.emplace_back();
    while (end != std::string::npos) {
      auto temp = line.substr(start, end - start);
      std::sort(std::begin(temp), std::end(temp));
      if (temp == "|") {
        val_lhs = false;
      }
      else if (val_lhs) {
        total.back().push_back(temp);
      } else {
        rhs.back().push_back(temp);
        total.back().push_back(temp);
      }
      start = end + 1;
      end = line.find(delimiter, start);
    }
    auto temp = line.substr(start, line.size() - start);
    std::sort(std::begin(temp), std::end(temp));
    rhs.back().push_back(temp);
    total.back().push_back(temp);
  }

  long long sum = 0;
  std::unordered_set<int> all = {0,1,2,3,4,5,6,7,8,9};

  for (int line_n = 0; line_n < total.size(); line_n++) {
    std::unordered_map<std::string, std::unordered_set<int>> map;
    for (const auto s : total[line_n]) {
      map[s] = all;
    }
    std::unordered_map<int, std::string> map_is;

    // Unique numbers
    for (const auto& s : total[line_n]) {
      if (s.size() == 2) {
        map[s] = {1};
        map_is[1] = s;
      }else if (s.size() == 4) {
        map[s] = {4};
        map_is[4] = s;
      }else if (s.size() == 3) {
        map[s] = {7};
        map_is[7] = s;
      }else if (s.size() == 7) {
        map[s] = {8};
        map_is[8] = s;
      }
    }

    // Next set of numbers that can be deduced
    for (const auto& s : total[line_n]) {
      if (s.size() == 6 && map_is.find(1) != map_is.end()) {
        if (count_common_elements(map_is[1], s) != 2) {
          map[s] = {6};
          map_is[6] = s;
        } else {
          if (map_is.find(4) != map_is.end()) {
            if (count_common_elements(map_is[4], s) == 4) {
              map[s] = {9};
              map_is[9] = s;
            } else {
              map[s] = {0};
              map_is[0] = s;
            }
          }
        }
      }
    }

    // Next set of numbers that can be deduced
    for (const auto& s : total[line_n]) {
      if (s.size() == 5 && map_is.find(1) != map_is.end()) {
        if (count_common_elements(map_is[1], s) == 2){
          map[s] = {3};
          map_is[3] = s;
        } else {
          if (map_is.find(9) != map_is.end()) {
            if (count_common_elements(map_is[9], s) == 5) {
              map[s] = {5};
              map_is[5] = s;
            } else if (count_common_elements(map_is[9], s) == 4) {
              map[s] = {2};
              map_is[2] = s;
            }
          }
        }
      }
    }

    // process of elimination
    std::unordered_set<int> to_remove;
    std::unordered_map<std::string, int> f;

    auto all_zero = [](const auto& m) {
      for (const auto p : m) {
        if (!p.second.empty()) {
          return false;
        }
      }
      return true;
    };

    auto temp = total[line_n];
    while(!all_zero(map)) {
      for (auto& p : map) {
        if (p.second.size() == 1) {
          to_remove.insert(*p.second.begin());
          f[p.first] = *p.second.begin();
        }
      }

      for (auto& p : map) {
        for (const auto ele : to_remove) {
          p.second.erase(ele);
        }
      }
    }

    int num = 0;
    for (const auto& s : rhs[line_n]) {
      num = num * 10 + f[s];
    }
    sum += num;
  }

  std::cout << sum << '\n';
  return 0;
}
