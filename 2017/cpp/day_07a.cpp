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

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_07_input" ;  
  std::ifstream file(input);
  std::string line;
  const std::regex pattern(R"(([a-z]+) \(([0-9]+)\)([a-z\ \,\-\>]?+))");
  // std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> map;
  std::unordered_set<std::string> supported_names_set;
  std::unordered_set<std::string> base_names_set;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    base_names_set.insert(match[1]);
    const std::string optional_supported_names = match[3];
    const std::vector<std::string> supported_names = !optional_supported_names.empty() ? 
      extract_supported_programs(optional_supported_names.substr(4, optional_supported_names.size() - 4)) : std::vector<std::string>();
    for (const auto& supported_name : supported_names) {
      supported_names_set.insert(supported_name);
    }
  }
  
  for (const auto& base_name : base_names_set) {
    if (supported_names_set.find(base_name) == supported_names_set.end()) {
      std::cout << base_name << '\n';
      break;
    }
  }
  return 0;
}