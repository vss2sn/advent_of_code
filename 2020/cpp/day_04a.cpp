#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  std::ifstream file;
  file.open("../input/day_04_input");
  std::string text;
  int count = 0;
  int fields = 0;
  std::unordered_set<std::string> key_fields{"byr", "iyr", "eyr", "hgt",
                                             "hcl", "ecl", "pid"};
  while (std::getline(file, text)) {
    text.erase(std::remove_if(std::begin(text), std::end(text),
                              [](char c) { return !std::isprint(c); }),
               std::end(text));
    if (text.empty()) {
      if (fields == 7) {
        ++count;
      }
      fields = 0;
      continue;
    }
    const std::string delimiter = " :";
    size_t start = 0;
    size_t end = text.find_first_of(delimiter);
    while (end != std::string::npos) {
      const std::string substr = text.substr(start, end - start);
      if (key_fields.find(substr) != key_fields.end()) {
        ++fields;
      }
      start = end + 1;
      end = text.find_first_of(delimiter, start);
    }
  }
  if (fields == 7) {
    ++count;
  }
  std::cout << count << '\n';
  return count;
}
