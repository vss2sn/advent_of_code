#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
struct Rule {
  std::string name;
  int min_1;
  int max_1;
  int min_2;
  int max_2;

  bool isValid(const int val) const {
    return (val >= min_1 && val <= max_1) || (val >= min_2 && val <= max_2);
  }
};

int main() {
  std::ifstream file{"day_16_data.txt"};
  std::string line;

  // tore rules
  std::vector<Rule> rules;
  const std::regex rule_pattern(R"(([a-zA-Z ]+): (\d+)-(\d+) or (\d+)-(\d+)(.*))");
  while(std::getline(file, line)) {
    if(line == "") break;
    std::smatch rule_match;
    std::regex_match(line, rule_match, rule_pattern);
    Rule new_rule{rule_match[1],
      std::stoi(rule_match[2]),
      std::stoi(rule_match[3]),
      std::stoi(rule_match[4]),
      std::stoi(rule_match[5])};
    rules.emplace_back(std::move(new_rule));
  }


  // store ticekts
  const std::regex ticket_pattern(R"((\d+),?)");
  std::vector<std::vector<int>> tickets;
  while(std::getline(file, line)) {
    if(auto it = std::remove_if(std::begin(line), std::end(line), [](const char c) { return !isprint(c); } ); it != std::end(line)) {
      line.erase(it);
    }
    auto start = std::sregex_iterator(std::begin(line), std::end(line), ticket_pattern);
    const auto end = std::sregex_iterator();
    std::vector<int> fields;
    while(start != end) {
      fields.push_back(std::stoi(std::smatch(*start).str()));
      start++;
    }
    tickets.emplace_back(std::move(fields));
  }

  // check whether invalid and add to sum
  int sum = 0;
  for(const auto& ticket: tickets) {
    for(const auto val : ticket) {
      if(!std::any_of(std::begin(rules), std::end(rules), [=](const auto& rule) { return rule.isValid(val); } )) {
        sum += val;
      }
    }
  }
  std::cout << sum << '\n';
  return sum;
}
