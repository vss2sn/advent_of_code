#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_10_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  const auto is_open_b = [](const auto c) {
    return c == '(' ||
           c == '[' ||
           c == '{' ||
           c == '<';
  };

  const auto match = [](const auto c1, const auto c2) {
    if (c1 == '(') return c2 == ')';
    else if(c1 == '[') return c2 == ']';
    else if(c1 == '{') return c2 == '}';
    else if(c1 == '<') return c2 == '>';
    return false;
  };

  const auto get_match = [](const auto c) {
    if (c == '(') return ')';
    else if(c == '[') return ']';
    else if(c == '{') return '}';
    else if(c == '<') return '>';
    return ' ';
  };

  const auto lookup_score = [&](const char c) {
    if (c == ')') return 1;
    else if (c == ']') return 2;
    else if (c == '}') return 3;
    else if (c == '>') return 4;
    else return 0;
  };

  std::vector<long long> auto_complete_scores;
  while(std::getline(file, line)) {
    std::stack<char> s;
    bool corrupt = false;
    for (const auto c : line) {
      if (is_open_b(c)) {
        s.push(c);
      } else {
        if (!match(s.top(), c)) {
          corrupt = true;
          break;
        } else {
          s.pop();
        }
      }
    }
    if (!corrupt) {
      long long auto_complete_score = 0;
      while(!s.empty()) {
        const char c = s.top();
        s.pop();
        auto_complete_score = auto_complete_score * 5 + lookup_score(get_match(c));
      }
      auto_complete_scores.push_back(auto_complete_score);
    }
  }

  std::sort(std::begin(auto_complete_scores), std::end(auto_complete_scores));
  std::cout << auto_complete_scores[auto_complete_scores.size()/2] << '\n';
  return 0;
}
