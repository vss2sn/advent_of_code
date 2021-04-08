#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// No need to change the input data
const std::map<int, std::string> update_rules{{8, "8: 42 | 42 8"},
                                              {11, "11: 42 31 | 42 11 31"}};

int main() {
  std::fstream file{"../input/day_19_input"};
  std::string s;
  std::map<int, std::string> known_rules;
  std::map<int, std::vector<std::vector<int>>> subsections_of_rules;
  std::vector<std::string> messages;
  // TODO(vss): improve the way line is read
  while (std::getline(file, s)) {
    s.erase(std::remove_if(std::begin(s), std::end(s),
                           [](const char c) { return !isprint(c); }),
            std::end(s));
    if (s.find(":") != std::string::npos) {
      int rule_lhs = std::stoi(s.substr(0, s.find(":")));
      if (const auto it = update_rules.find(rule_lhs);
          it != update_rules.end()) {
        s = it->second;
      }
      if (s.find("\"") != std::string::npos) {
        std::string suffix = s.substr(s.find("\"") + 1, 1);
        known_rules[rule_lhs] = suffix;
      } else {
        s = s.substr(s.find(":") + 2);
        std::vector<int> rule_section;
        int num_in_rule_section = 0;
        for (char c : s) {
          if (c == ' ') {
            if (num_in_rule_section > 0) {
              rule_section.push_back(num_in_rule_section);
            }
            num_in_rule_section = 0;
          } else if (c == '|') {
            subsections_of_rules[rule_lhs].push_back(rule_section);
            rule_section.clear();
          } else {
            num_in_rule_section = (num_in_rule_section * 10) + (c - '0');
          }
        }
        rule_section.push_back(num_in_rule_section);
        subsections_of_rules[rule_lhs].push_back(rule_section);
      }
    } else {
      messages.push_back(s);
    }
  }
  std::map<std::pair<std::string, int>, bool> previously_seen_pairs;
  std::function<bool(std::string, int)> DoesMsgFollowRule =
      [&previously_seen_pairs, &known_rules, &subsections_of_rules,
       &DoesMsgFollowRule](const std::string& s, const int source) {
        const int n = s.size();
        // If the rule is a known rule, return whether the string matches the
        // known rule
        if (known_rules.count(source)) {
          return (s == known_rules[source]);
        }
        // If a particular {pattern, rule index} pair has already been seen
        // before, retuurn the stored result
        if (previously_seen_pairs.count({s, source})) {
          return previously_seen_pairs[{s, source}];
        }
        // Iterate over each subsection to check whether the string satisfies
        // any of the subsections
        for (auto& subsection : subsections_of_rules[source]) {
          const size_t parts = subsection.size();
          if (parts == 1) {
            // If 1 number in subsection, check whether the string satisfies the
            // corresponding rule
            if (DoesMsgFollowRule(s, subsection[0])) {
              return previously_seen_pairs[{s, source}] = true;
            }
          } else if (parts == 2) {
            // If 2 numbers in subsection, check whether the string can be
            // broken in a way to satisfy both numbers in the subsection
            // simultaneously
            for (int i = 0; i < n - 1; i++) {
              if (DoesMsgFollowRule(s.substr(0, i + 1), subsection[0]) &&
                  DoesMsgFollowRule(s.substr(i + 1), subsection[1])) {
                return previously_seen_pairs[{s, source}] = true;
              }
            }
          } else if (parts == 3) {
            // If 3 numbers in subsection, check whether the string can be
            // broken in a way to satisfy all 3 numbers in the subsection
            // simultaneously
            for (int i = 0; i < n; i++) {
              for (int j = i + 1; j < n; j++) {
                if (DoesMsgFollowRule(s.substr(0, i + 1), subsection[0]) &&
                    DoesMsgFollowRule(s.substr(i + 1, j - i), subsection[1]) &&
                    DoesMsgFollowRule(s.substr(j + 1, n - j), subsection[2])) {
                  return previously_seen_pairs[{s, source}] = true;
                }
              }
            }
          } else {
            std::cout << "This should not happen" << '\n';
          }
        }
        return previously_seen_pairs[{s, source}] = false;
      };
  int res = 0;
  for (const std::string& message : messages) {
    if (DoesMsgFollowRule(message, 0)) {
      ++res;
    }
    previously_seen_pairs.clear();
  }
  std::cout << res << '\n';
  return 0;
}
