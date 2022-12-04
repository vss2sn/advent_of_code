#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

std::tuple<std::string, std::string> split(const std::string& str, const char delim) {
  const auto delim_i = str.find(delim);
  const auto s1 = str.substr(0, delim_i);
  const auto s2 = str.substr(delim_i + 1, str.size() - delim_i);
  return {s1, s2};
}

std::tuple<int, int> split_with_int(const std::string& str, const char delim) {
  const auto [s1, s2] = split(str, delim);
  return {std::stoi(s1), std::stoi(s2)};
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_04_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  int total = 0;
  while(std::getline(file, line)) {
    // Account for CRLF if required
    // line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    const auto [e1, e2] = split(line, ',');
    const auto [min_1, max_1] = split_with_int(e1, '-');
    const auto [min_2, max_2] = split_with_int(e2, '-');
    if ((min_1 >= min_2 && max_1 <= max_2) ||
        (min_1 <= min_2 && max_1 >= max_2)) {
      total += 1;
    }
  }
  std::cout << total << '\n';
  return 0;
}
