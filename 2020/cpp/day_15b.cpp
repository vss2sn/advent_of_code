#include <fstream>
#include <iostream>
#include <unordered_map>

int main() {
  std::fstream file{"../input/day_15_input"};
  std::string line;
  std::getline(file, line);
  const std::string delimiter = ",";
  size_t start = 0;
  std::size_t end = line.find(delimiter);
  std::unordered_map<int, int> spoken_numbers;
  while (end != std::string::npos) {
    spoken_numbers.insert(
        {std::stoi(line.substr(start, end - start)), spoken_numbers.size()});
    start = end + delimiter.size();
    end = line.find(delimiter, start);
  }
  // spoken_numbers.insert({std::stoi(line.substr(start, end - start)),
  // spoken_numbers.size()}); std::cout << std::stoi(line.substr(start, end -
  // start)) << '\n';
  long long new_number = std::stoi(line.substr(start, end - start));
  size_t count = spoken_numbers.size();
  while (count < 29999999) {
    if (auto it = spoken_numbers.find(new_number); it != spoken_numbers.end()) {
      new_number = count - spoken_numbers[new_number];
      it->second = count;
    } else {
      spoken_numbers.insert({new_number, count});
      new_number = 0;
    }
    count++;
  }
  std::cout << new_number << '\n';
  return new_number;
}
