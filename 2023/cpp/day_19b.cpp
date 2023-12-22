#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <memory>

using PartRating = std::array<int, 4>;

struct Conditional {
  bool gt; // greater than
  int value; // value to compare against
  int index; // refers to the variable being compared (x=0, m=1, a=2, s=3)
  std::pair<std::string, std::unique_ptr<Conditional>> is_true;
  std::pair<std::string, std::unique_ptr<Conditional>> is_false;
  bool evaluate(const PartRating& pr) {
    if (gt) {
      return pr[index] > value;
    }
    return pr[index] < value;
  }
};

int get_idx(std::string var) {
  if (var == "x") return 0;
  if (var == "m") return 1;
  if (var == "a") return 2;
  if (var == "s") return 3; 
  std::cout << "This should not happen" << '\n';
  exit(0);
  return 0;
}

PartRating parse_input(const std::string& line) {
  const std::regex pattern(R"(\{x=([0-9]+),m=([0-9]+),a=([0-9]+),s=([0-9]+)\})");
  std::smatch match;
  std::regex_search(line, match, pattern); 
  PartRating pr;
  pr[0] = std::stoi(match[1]);
  pr[1] = std::stoi(match[2]);
  pr[2] = std::stoi(match[3]);
  pr[3] = std::stoi(match[4]);
  return pr;
}

Conditional parse_conditionals (const std::string& line) {
  if (line.empty()) {
    return Conditional();
  }
  const std::regex pattern(R"(([xmas])([<>])([0-9]+):([a-zA-Z]+),(.*))");
  std::smatch match;
  std::regex_search(line, match, pattern); 
  Conditional conditional;
  conditional.value = std::stoi(match[3]);
  conditional.index = get_idx(match[1]) ;
  conditional.gt = match[2] == ">" ? true : false;
  if (std::string(match[4]).find(':') == std::string::npos) {
    conditional.is_true = std::pair{match[4], std::unique_ptr<Conditional>()};
    // conditional.is_true.second.reset(); // sanity check
  } else {
    conditional.is_true = std::pair{match[4], std::make_unique<Conditional>(parse_conditionals(match[4]))};
  }
  if (std::string(match[5]).find(':') == std::string::npos) {
    conditional.is_false = std::pair{match[5], std::unique_ptr<Conditional>()};
    // conditional.is_false.second.reset(); // sanity check
  } else {
    conditional.is_false = std::pair{match[5], std::make_unique<Conditional>(parse_conditionals(match[5]))};
  }
  return conditional;
}

std::pair<std::string, Conditional> parse_workflow(const std::string& line) {
  const auto idx_open_brace = line.find('{');
  const auto name = line.substr(0, idx_open_brace);
  Conditional conditional = parse_conditionals(line.substr(idx_open_brace + 1, line.size() - 1 - (idx_open_brace + 1)));
  std::pair<std::string, Conditional> p;
  p.first = name;
  p.second = std::move(conditional);
  return p;
}

struct AcceptableValues {
  PartRating min;
  PartRating max;
};

void solve(
  std::unordered_map<std::string, Conditional>& workflows,
  Conditional* conditional,
  const std::string& name,
  AcceptableValues acceptable_values,
  std::vector<AcceptableValues>& results
) {
  if (!name.empty()) conditional = &workflows[name];
  // Split the current acceptable values into 2, meeting the condition and not meeting the condition
  std::array<AcceptableValues, 2> split; 
  split[0] = acceptable_values; // meets condition
  split[1] = acceptable_values; // does not meet condition
  if (conditional->gt) {
    split[0].min[conditional->index] = std::max(conditional->value + 1, split[0].min[conditional->index]);
    split[1].max[conditional->index] = std::min(conditional->value, split[1].max[conditional->index]);
  } else {
    split[0].max[conditional->index] = std::min(conditional->value - 1, split[0].max[conditional->index]);
    split[1].min[conditional->index] = std::max(conditional->value, split[1].min[conditional->index]);
  }
  // util function to move to the next conditional to check
  const auto util = [&workflows,conditional, &results](const bool is_true, AcceptableValues acceptable_values_updated) {
    auto& next_pair = is_true ? conditional->is_true : conditional->is_false;;
    if (next_pair.second.get() == nullptr) {
      if (next_pair.first == "A") {
        results.push_back(acceptable_values_updated);
        return;
      }
      if(next_pair.first == "R") {
        return;
      }
      solve (workflows, nullptr, next_pair.first, acceptable_values_updated, results);
      return; 
    }
    solve(workflows, next_pair.second.get(), "", acceptable_values_updated, results);    
  };
  util(true, split[0]);
  util(false, split[1]);
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::unordered_map<std::string, Conditional> workflows;
  while(std::getline(file, line)) {
    if (line.empty()) break;
    auto [name, conditional] = parse_workflow(line);
    workflows[name] = std::move(conditional);
  }

  std::vector<AcceptableValues> results;
  AcceptableValues acceptable_values;
  for (int i = 0; i < 4; i++) {
    acceptable_values.min[i] = 1;
    acceptable_values.max[i] = 4000;
  }
  solve(workflows, nullptr, "in", acceptable_values, results);
  // for (const auto & result : results ) {
  //   for (int i = 0; i < 4; i++) {
  //     std::cout << "(" << result.min[i] << ',' << result.max[i] << ")" << " , ";
  //   }
  //   std::cout << '\n';
  // }
  std::size_t total = 0;
  for (const auto& result : results) {
    std::size_t result_total = 1;
    for (int i = 0; i < 4; i++) {
      result_total *= (result.max[i] - result.min[i] + 1); // Include the ends
    }
    total += result_total;
  }
  std::cout << total << '\n';
  return 0;
}