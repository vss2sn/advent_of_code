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
  bool gt;
  int value;
  int index;
  std::pair<std::string, std::unique_ptr<Conditional>> is_true;
  std::pair<std::string, std::unique_ptr<Conditional>> is_false;
  bool evaluate(const PartRating& pr) {
    if (gt) {
      return pr[index] > value;
    }
    return pr[index] < value;
  }
};

bool compare(const int v1, const int v2, std::string sign) {
  if (sign == ">") {
    return v1 > v2;
  }
  return v1 < v2;
}

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

PartRating temp;
int count = 0;
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
    conditional.is_true.second.reset();
  } else {
    conditional.is_true = std::pair{match[4], std::make_unique<Conditional>(parse_conditionals(match[4]))};
  }
  if (std::string(match[5]).find(':') == std::string::npos) {
    // std::cout << "5: " << match[5] << '\n';
    conditional.is_false = std::pair{match[5], std::unique_ptr<Conditional>()};
    conditional.is_false.second.reset();
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

std::string solve(
  const PartRating& pr, 
  std::unordered_map<std::string, Conditional>& workflows,
  Conditional* conditional,
  const std::string& name) {
  if (!name.empty()) conditional = &workflows[name];
  auto ans = conditional->evaluate(pr);
  auto& next_pair = ans ? conditional->is_true : conditional->is_false;;
  if (next_pair.second.get() == nullptr) {
    if (next_pair.first == "A" || next_pair.first== "R") return next_pair.first;
    return solve (pr, workflows, nullptr, next_pair.first);
  }
  return solve(pr, workflows, next_pair.second.get(), "");    
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::string> map;
  std::unordered_map<std::string, Conditional> workflows;
  while(std::getline(file, line)) {
    if (line.empty()) break;
    auto [name, conditional] = parse_workflow(line);
    workflows[name] = std::move(conditional);
  }
  std::vector<PartRating> parts;
  while(std::getline(file, line)) {
    parts.emplace_back(parse_input(line));
  }

  std::size_t total = 0;
  for (const auto pr : parts) {
    std::string ans = solve(pr, workflows, nullptr, "in");
    if (ans == "A") {
      for (const auto ele : pr) {
        total += ele;
      }
    }
  }
  std::cout << total << '\n';
  return 0;
}