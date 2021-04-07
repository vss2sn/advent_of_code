#include <algorithm>
// #include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main() {
  std::ifstream file;
  file.open("../input/day_4_input");
  std::string text;
  int count = 0;
  int fields = 0;
  std::unordered_set<std::string> key_fields{"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
  std::unordered_map<std::string, bool(*)(const std::string&)> rules {
    {"byr", [](const std::string& val) { const int i_val = std::stoi(val); return i_val >= 1920 && i_val <= 2002;} },
    {"iyr", [](const std::string& val) { const int i_val = std::stoi(val); return i_val >= 2010 && i_val <= 2020;} },
    {"eyr", [](const std::string& val) { const int i_val = std::stoi(val); return i_val >= 2020 && i_val <= 2030;} },
    {"hgt", [](const std::string& val) {
        const std::string unit = val.substr(val.size() - 2, 2);
        if(unit == "cm") {
          const int i_val = stoi(val.substr(0, val.size() - 2));
          return i_val >= 150 && i_val <= 193;
        }
        else if(unit == "in") {
          const int i_val = stoi(val.substr(0, val.size() - 2));
          return i_val >= 59 && i_val <= 76;
        }
        else return false;
      }
    },
    {"hcl", [](const std::string& val) {
        return val.size() == 7 &&
          val[0] == '#' &&
          (val.substr(1, val.size()).find_first_not_of("0123456789abcdef") == std::string::npos);
      }
    },
    {"ecl", [](const std::string& val) {
      const std::unordered_set<std::string> allowed_vals{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
      return allowed_vals.find(val) != allowed_vals.end();}
    },
    {"pid", [](const std::string& val) {
        return val.size() == 9 &&
        (val.find_first_not_of("0123456789") == std::string::npos);
      }
    }
  };

  while(std::getline(file, text)) {
    text.erase(std::remove_if(std::begin(text), std::end(text), [](char c) { return !std::isprint(c); }));
    if(text.empty()) {
      if(fields == 7) {
        ++count;
      }
      fields = 0;
      continue;
    }

    const std::string delimiter = " ";
    size_t start = 0;
    size_t end = text.find_first_of(delimiter);
    while(end != std::string::npos) {
      const size_t pos_colon = text.find(':', start);
      const std::string key = text.substr(start, pos_colon - start);
      const std::string val = text.substr(pos_colon + 1, end - pos_colon - 1);
      if(key_fields.find(key) != key_fields.end() && rules[key](val)) {
        ++fields;
      }
      start = end + 1;
      end = text.find_first_of(delimiter, start);
    }
    const size_t pos_colon = text.find(':', start);
    const std::string key = text.substr(start, pos_colon - start);
    const std::string val = text.substr(pos_colon + 1, text.size() - pos_colon - 1);
    if(key_fields.find(key) != key_fields.end() && rules[key](val)) {
      ++fields;
    }
    start = end + 1;
    end = text.find_first_of(delimiter, start);
  }

  if(fields == 7) {
    ++count;
  }
  std::cout << count << '\n';
  return count;
}
