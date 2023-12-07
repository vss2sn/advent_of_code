#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Mapping {
  long long destination_start;
  long long source_start;;
  long long range;

  bool in_destination_range(const long long n) const {
    return n >= destination_start && n <= destination_start + range;
  }

  bool in_source_range(const long long n) const {
    return n >= source_start && n <= source_start + range;
  }

  long long get_mapping(const long long n) const {
    return destination_start + (n - source_start);
  }
};

struct Map {
  std::string map_from;
  std::string map_to;
  std::vector<Mapping> mappings;
};

std::vector<long long> extract(const std::string& s) {
  std::vector<long long> numbers;
  long long start = 0;
  long long end = s.find(' ');
  while (end != std::string::npos) {
    // std::cout << s.substr(start, end - start) << '|' << '\n';
    numbers.push_back(std::stoll(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(' ', start);
  }
  // std::cout << s.substr(start, s.size() - start) << '|' << '\n';
  numbers.push_back(std::stoll(s.substr(start, s.size() - start)));
  return numbers;
}

std::vector<std::string> parameters {
  "soil", "fertilizer", "water", "light", "temperature", "humidity", "location"
};

long long get_location (long long value, const std::vector<Map>& maps, const std::string& map_from) {
  if (map_from == "location") {
    return value;
  }
  const auto map = *std::find_if(
    std::begin(maps), 
    std::end(maps), 
    [&map_from](const auto& map) {
      return map.map_from == map_from;
    }
  );
  for (const auto& mapping : map.mappings) {
    if (mapping.in_source_range(value)) {
      return get_location(mapping.get_mapping(value), maps, map.map_to);
    } 
  }
  return get_location(value, maps, map.map_to);
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
  // std::cout << line << '\n';
  std::vector<long long> seeds = extract(line.substr(7, line.size() - 7));

  while(std::getline(file, line)) {
    if (line.empty()) {
      std::getline(file, line);
        // std::cout << line << '\n';

      Map map;
      long long start = 0;
      long long end = line.find('-');
      map.map_from = line.substr(start, end - start);
      start = end + 4;
      end = line.find(' ', start);
      map.map_to = line.substr(start, end - start);
      std::cout << map.map_from << " ---> " << map.map_to << '\n';
      maps.push_back(map);
      std::getline(file, line);
    }
      // std::cout << line << '\n';

    const auto numbers = extract(line);
    Mapping mapping;
    mapping.destination_start = numbers[0];
    mapping.source_start = numbers[1];
    mapping.range = numbers[2];
    // for (const auto number : numbers) {
    //   std::cout << number << ' ';
    // }
    // std::cout << '\n';
    maps.back().mappings.push_back(mapping);
  }

  std::vector<long long> locations;
  for (const auto& seed : seeds) {
    locations.push_back(get_location(seed, maps, "seed"));
    std::cout << locations.back() << '\n';
  }
  std::cout << *std::min_element(std::begin(locations), std::end(locations)) << '\n';
  return 0;
}