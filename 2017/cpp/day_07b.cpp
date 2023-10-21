#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>

std::vector<std::string> extract_supported_programs(const std::string& s) {
  std::vector<std::string> supported_programs;
  std::size_t start = 0;
  const std::string delimiter = ", ";
  std::size_t end = s.find(delimiter);
  while(end != std::string::npos) {
    supported_programs.emplace_back(s.substr(start, end - start));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
  }
  supported_programs.emplace_back(s.substr(start, end - start));
  return supported_programs;
}

// Create a weight map that stores the weight of the proram and all programas on its disc. Find one program on whose disk the weights are unbalanced and return the weight it should be for the discs to balance
std::pair<bool, int> create_weight_map(std::unordered_map<std::string, std::pair<int, std::vector<std::string>>>& map, const std::string& base_name, std::unordered_map<std::string, int>& weight_map) {
  const auto& [weight, supported_names] = map[base_name];
  if (supported_names.empty()) {
    weight_map[base_name] = weight;
    return {false, 0};
  }
  std::unordered_map<int, std::vector<std::string>> temp; 
  weight_map[base_name] =  map[base_name].first;
  for(const auto& supported_name : supported_names) {
    if (weight_map.find(supported_name) == weight_map.end()) {
      const auto result = create_weight_map(map, supported_name, weight_map);
      if (result.first) return result;
    }
    weight_map[base_name] += weight_map[supported_name];

    if (temp.find(weight_map[supported_name]) == temp.end()) {
      temp[weight_map[supported_name]] = std::vector<std::string>{supported_name};
    } else{
      temp[weight_map[supported_name]].push_back(supported_name);
    }
  }
  if (temp.size() > 1) {
    int ans = 0;
    for (const auto& [weight, names] : temp) {
      if (names.size() == 1) {
        ans += -weight + map[names[0]].first;
      } else {
        ans += weight;
      }
    }
    return {true, ans}; 
  }
  return {false, 0};
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_07_input" ;  
  std::ifstream file(input);
  std::string line;
  const std::regex pattern(R"(([a-z]+) \(([0-9]+)\)([a-z\ \,\-\>]?+))");
  std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> map;
  std::unordered_set<std::string> supported_names_set;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    const std::string base = match[1];
    const int weight = std::stoi(match[2]);
    const std::string optional_supported_names = match[3];
    const std::vector<std::string> supported_names = !optional_supported_names.empty() ? 
      extract_supported_programs(optional_supported_names.substr(4, optional_supported_names.size() - 4)) : std::vector<std::string>();
    map.insert({base, {weight, supported_names}});
    for (const auto& ele : supported_names) {
      supported_names_set.insert(ele);
    }
  }
  
  std::string main_base_name;
  for (const auto& [name, data] : map) {
    if (supported_names_set.find(name) == supported_names_set.end()) {
      main_base_name = name;
    }
  }

  std::unordered_map<std::string, int> weight_map;
  const auto result = create_weight_map(map, main_base_name, weight_map);
  std::cout << result.second << '\n';
  return 0;
}