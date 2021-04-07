#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_set>
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
  std::ifstream file{"../input/day_16_input"};
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
    if(line == "") {
      continue;
    }
    auto start = std::sregex_iterator(std::begin(line), std::end(line), ticket_pattern);
    const auto end = std::sregex_iterator();
    std::vector<int> fields;
    while(start != end) {
      fields.push_back(std::stoi(std::smatch(*start).str()));
      start++;
    }
    if(fields.size() == 0) continue;
    tickets.emplace_back(std::move(fields));
  }

  std::vector<bool> valid(tickets.size(), true);
  for(int i = 0; i < tickets.size(); ++i) {
    for(auto val : tickets[i]) {
      if(!std::any_of(std::begin(rules), std::end(rules), [=](const auto& rule) { return rule.isValid(val); } )) {
        valid[i] = false;
      }
    }
  }

  std::unordered_set<int> temp;
  for(int i = 0; i < tickets[0].size(); ++i) {
    temp.insert(i);
  }

  // Set all possible fields in field palcement matrix, where rtow s the fueld and the set contains all teh possible posiitions on the ticket it can be assigned to
  // Then check each value on every ticket against the rules to remove placements that are not possible
  std::vector<std::unordered_set<int>> field_placement_matrix(rules.size(), temp);
  for (int k = 0; k < tickets.size(); k++) {
    for (int i = 0; i < tickets[k].size(); i++) {
      if(valid[k]) {
        for(int j = 0; j < rules.size(); j++) {
          if (!rules[j].isValid(tickets[k][i])) {
            field_placement_matrix[j].erase(i);
          }
        }
      }
    }
  }

  // Find the field that has only 1 possible position, remove said position from all fields. Iterate until all the fields are correctly assigned
  std::map<int, int> final_mapping;
  while(std::any_of(std::begin(field_placement_matrix), std::end(field_placement_matrix), [](const auto v) {return v.size();})) {
    for(int i = 0; i < field_placement_matrix.size(); ++i) {
      if(field_placement_matrix[i].size() == 1) {
        final_mapping.insert({i, *field_placement_matrix[i].begin()});
        const auto val_to_erase = *field_placement_matrix[i].begin();
        for(auto& set: field_placement_matrix) {
          if(auto it = set.find(val_to_erase); it != set.end()) {
            set.erase(it);
          }
        }
      }
    }
  }

  long long product = 1;
  for(int i = 0; i < rules.size(); ++i) {
    if(rules[i].name.substr(0, 9) == "departure") {
      product *= tickets[0][final_mapping[i]];
    }
  }

  std::cout << product << '\n';
  return product;
}
