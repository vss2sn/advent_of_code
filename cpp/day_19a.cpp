#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void generatePermutations(const std::string& cur,
  const int index,
  const std::unordered_map<int, std::vector<std::string>>& known_rules,
  const std::vector<int>& rules_indices,
  std::vector<std::string>& permutations) {

  if (index < rules_indices.size()) {
    const auto& it = known_rules.find(rules_indices[index]);
    for (const auto& ele : (it->second)) {
      generatePermutations(cur + ele, index + 1, known_rules, rules_indices, permutations);
    }
  } else {
    permutations.push_back(cur);
  }
}

void SimplifyRules(
  std::unordered_map<int, std::vector<std::string>>& known_rules,
  std::unordered_map<int, std::string>& unknown_rules,
  const int rule_to_simplify) {

  if ( const auto it = known_rules.find(rule_to_simplify); it == known_rules.end()) {
    auto& rule = unknown_rules[rule_to_simplify];
    std::vector<std::vector<int>> patterns(1, std::vector<int>());
    if (std::all_of(std::begin(rule), std::end(rule), [](const char c){ return std::isdigit(c); })) {
      SimplifyRules(known_rules, unknown_rules, std::stoi(rule));
      patterns.back().push_back(std::stoi(rule));
    } else {
      std::size_t start = 0;
      std::size_t end = rule.find(' ');
      while(end != std::string::npos) {
        const auto sub = rule.substr(start, end - start);
        if (sub == "|") {
          patterns.emplace_back();
        } else {
          const int new_rule_index = std::stoi(sub);
          SimplifyRules(known_rules, unknown_rules, new_rule_index);
          patterns.back().push_back(new_rule_index);
        }
        start = end + 1;
        end = rule.find(' ', start);
      }
      const int new_rule_index = std::stoi(rule.substr(start, end - start));
      SimplifyRules(known_rules, unknown_rules, new_rule_index);
      patterns.back().push_back(new_rule_index);
    }
    std::vector<std::string> permutations;
    for(const auto& pattern : patterns) {
      generatePermutations("", 0, known_rules, pattern, permutations);
    }
    known_rules.insert({rule_to_simplify, permutations});
    unknown_rules.erase(rule_to_simplify);
  }
}

int main() {
  std::fstream file{"day_19_data.txt"};
  const std::regex rule_pattern_known(R"((\d+): \"([a-z]+)\")");
  const std::regex rule_pattern_unknown(R"((\d+): ([0-9| ]+))");
  std::unordered_map<int, std::vector<std::string>> known_rules;
  std::unordered_map<int, std::string> unknown_rules;
  std::string line;
  while(std::getline(file, line)) {
    if(line == "") break;
    std::smatch rule_match_known;
    std::smatch rule_match_unknown;
    std::regex_match(line, rule_match_known, rule_pattern_known);
    std::regex_match(line, rule_match_unknown, rule_pattern_unknown);
    if(rule_match_known.size() > 0) {
      known_rules.insert({std::stoi(rule_match_known[1]), {rule_match_known[2]}});
    } else if (rule_match_unknown.size() > 0) {
      unknown_rules.insert({std::stoi(rule_match_unknown[1]), {rule_match_unknown[2]}});
    } else {
      break;
    }
  }

  while(unknown_rules.size() > 0) {
    SimplifyRules(known_rules, unknown_rules, 0);
  }

  int count = 0;
  std::unordered_set<std::string> patterns_of_zero(std::begin(known_rules[0]), std::end(known_rules[0]));
  while(std::getline(file, line)) {
    if(line == "") continue;
    if(patterns_of_zero.find(line) != patterns_of_zero.end()) {
      count++;
    }
  }
  std::cout << count << '\n';
  return count;
}
