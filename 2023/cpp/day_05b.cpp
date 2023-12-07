#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// use range of seed values instead of single seed values
// keep extracting the parts of the range on which mappings from the next map are applied to get the new value
// move forward the values that were part of the range on which mappings from the next map are not applied without modification
// Iterate over all the maps until the location values are reached. 
// Find the min location.

struct Mapping {
  std::array<long long, 2> from;
  std::array<long long, 2> to;

  long long apply_map(const long long n) const {
    return to[0] + (n - from[0]);
  }
};

struct Map {
  std::string map_from;
  std::string map_to;
  std::vector<Mapping> mappings;
};

std::vector<long long> extract(const std::string& s) {
  std::vector<long long> numbers;
  std::size_t start = 0;
  std::size_t end = s.find(' ');
  while (end != std::string::npos) {
    numbers.push_back(std::stoll(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(' ', start);
  }
  numbers.push_back(std::stoll(s.substr(start, s.size() - start)));
  return numbers;
}

std::vector<std::pair<long long, long long>> apply_filter(std::vector<std::pair<long long, long long>> relevant_ranges, const std::vector<Mapping>& mappings) {
  std::vector<std::pair<long long, long long>> new_relevant_values;
//   for (const auto& p : relevant_ranges) {
//     std::cout << p.first << ' ' << p.second << '\n';
//   }

  for (const auto&  relevant_range : relevant_ranges) {
    // std::cout << "revelant_range: " << relevant_range.first << ' ' << relevant_range.second << '\n';
    std::vector<std::pair<long long, long long>> ranges_where_filter_applied;
    std::vector<std::pair<long long, long long>> values_when_filter_applied;
    for (const auto& mapping : mappings) {
    //   std::cout << "mapping: (" << mapping.from[0] << ',' << mapping.from[1] << ") --> (" << mapping.to[0] << ',' << mapping.to[1] << ")\n";
      if (relevant_range.second < mapping.from[0] || relevant_range.first > mapping.from[1]) continue;
    //   std::cout << "Applied" << '\n'; 
      if (relevant_range.first >= mapping.from[0] && relevant_range.second <= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(relevant_range.first, relevant_range.second);
      } else if (relevant_range.first >= mapping.from[0] && relevant_range.second >= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(relevant_range.first, mapping.from[1]);
      } else if (relevant_range.first <= mapping.from[0] && relevant_range.second <= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(mapping.from[0],relevant_range.second);
      } else if (relevant_range.first <= mapping.from[0] && relevant_range.second >= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(mapping.from[0], mapping.from[1]);
      } else {
        std::cout << "?!" << '\n';
        exit(0);
      }
      values_when_filter_applied.emplace_back(mapping.apply_map(ranges_where_filter_applied.back().first), mapping.apply_map(ranges_where_filter_applied.back().second));
    //   std::cout << "Range where filter applied: (" << ranges_where_filter_applied.back().first << ',' << ranges_where_filter_applied.back().second << ")\n";
    //   std::cout << "Value where filter applied: (" << values_when_filter_applied.back().first << ',' << values_when_filter_applied.back().second << ")\n";
    }
    auto current_start = relevant_range.first;
    for (int i = 0; i < ranges_where_filter_applied.size() ; i++) {
      // current_start will always be less that the range start
      new_relevant_values.emplace_back(current_start, ranges_where_filter_applied[i].first - 1);
      new_relevant_values.emplace_back(values_when_filter_applied[i].first, values_when_filter_applied[i].second);
      current_start = ranges_where_filter_applied[i].second + 1;
    //   std::cout << "Something was pushed back" << '\n';;
    }
    if (current_start <= relevant_range.second) {
      new_relevant_values.emplace_back(current_start, relevant_range.second);
    }
  }
  std::vector<std::pair<long long, long long>> filtered_new_relevant_values;
  std::copy_if (std::begin(new_relevant_values), std::end(new_relevant_values), std::back_inserter(filtered_new_relevant_values), [](const auto& ele ){
    // std::cout << ele.first << ' ' << ele.second << '\n';
    return ele.first <= ele.second; 
  } );
//   std::cout << new_relevant_values.size() << ' ' << filtered_new_relevant_values.size() << '\n';
  std::sort(std::begin(filtered_new_relevant_values), std::end(filtered_new_relevant_values), [](const auto& v1, const auto& v2) {return v1.first < v2.first;});
  // int i = 0;
  // while(i  < filtered_new_relevant_values.size() - 1) {
  //   if (filtered_new_relevant_values[i+1].first < filtered_new_relevant_values[i].second) {
  //     filtered_new_relevant_values[i].second = std::max(filtered_new_relevant_values[i+1].second, filtered_new_relevant_values[i].second);
  //     filtered_new_relevant_values.erase(std::begin(filtered_new_relevant_values) + i + 1);
  //   } else {
  //     i++;
  //   }
  // }
  // std::sort(std::begin(filtered_new_relevant_values), std::end(filtered_new_relevant_values), [](const auto& v1, const auto& v2) {return v1.first < v2.first;});
  return filtered_new_relevant_values;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_05_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Map> maps;
  std::getline(file, line);
  std::vector<long long> input_ranges = extract(line.substr(7, line.size() - 7));
  std::vector<std::pair<long long, long long>> seeds;
  for (int i = 0; i < input_ranges.size(); i+=2) {
    seeds.emplace_back(input_ranges[i], input_ranges[i] + input_ranges[i+1] - 1);
  }
  while(std::getline(file, line)) {
    if (line.empty()) {
      std::getline(file, line);
      Map map;
      long long start = 0;
      long long end = line.find('-');
      map.map_from = line.substr(start, end - start);
      start = end + 4;
      end = line.find(' ', start);
      map.map_to = line.substr(start, end - start);
      maps.push_back(map);
      std::getline(file, line);
    }
    const auto numbers = extract(line);
    Mapping mapping;
    mapping.to = {{numbers[0], numbers[0] + numbers[2]-1}};
    mapping.from = {{numbers[1], numbers[1] + numbers[2]-1}};
    // std::cout << mapping.from[0] << ' ' << mapping.from[1] << '\n';
    // std::cout << mapping.to[0] << ' ' << mapping.to[1] << '\n';
    maps.back().mappings.push_back(mapping);
  }

  for (auto& map : maps) {
    std::sort(std::begin(map.mappings), std::end(map.mappings), [](const auto& m1, const auto& m2) {return m1.from[0] < m2.from[0];});
  }

  for (int i = 0; i < maps.size()-1; i++) {
    assert(maps[i+1].map_from == maps[i].map_to);
  }

  std::vector<std::pair<long long, long long>> relevant_values = seeds;
  for (const auto& map : maps) {
    relevant_values = apply_filter(relevant_values, map.mappings);
    // std::cout << "------------------- " << '\n';
    // for (const auto& p : relevant_values) {
    //   std::cout << p.first << ' ' << p.second << '\n';
    // }
    // std::cout << " xxxxxxxxxxxxxxxxxxxxxxxxxx " << '\n';
  }

  std::cout << min_element(std::begin(relevant_values), std::end(relevant_values))->first << '\n';
  return 0;
}