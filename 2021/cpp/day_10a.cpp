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

  const auto lookup_score = [&](const char c) {
    if (c == ')') return 3;
    else if (c == ']') return 57;
    else if (c == '}') return 1197;
    else if (c == '>') return 25137;
    else return 0;
  };

  long long syntax_score = 0;
  while(std::getline(file, line)) {
    std::stack<char> s;
    bool corrupt = false;
    for (const auto c : line) {
      if (is_open_b(c)) {
        s.push(c);
      } else {
        if (!match(s.top(), c)) {
          corrupt = true;
          syntax_score += lookup_score(c);
          break;
        } else {
          s.pop();
        }
      }
    }
  }

  std::cout << syntax_score << '\n';
  return 0;
}
