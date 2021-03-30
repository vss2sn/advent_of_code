#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <unordered_set>

int main() {
  std::ifstream file;
  file.open("day_6_data.txt");

  const std::string full_string = "abcdefghijklmnopqrstuvwxyz";
  int count = 0;
  std::string line;
  std::string intersection = full_string;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line),
                 [](auto c) { return !isprint(c); }), std::end(line));
    if(line.empty()) {
      count += intersection.size();
      intersection = full_string;
    } else {
      std::sort(std::begin(line), std::end(line));
      std::string temp_intersection = "";
      std::set_intersection(std::begin(line), std::end(line),
        std::begin(intersection), std::end(intersection),
        std::back_inserter(temp_intersection));
      intersection = temp_intersection;
    }
  }
  count += intersection.size();

  std::cout << count << '\n';
  return count;
}
