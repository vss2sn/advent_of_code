#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

void print(const std::unordered_map<int, long long>& m) {
  for (const auto& p : m) {
    std::cout << p.first << ": " << p.second << '\n';
  }
  std::cout << '\n';
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  std::size_t start = 0;
  std::size_t end = line.find(',', start);
  std::unordered_map<int, long long> fish;
  for (int i = 0; i < 9; i++) {
    fish[i] = 0;
  }
  while (end != std::string::npos) {
    const auto val = std::stoll(line.substr(start, end - start));
    fish[val]++;
    start = end + 1;
    end = line.find(',', start);
  }
  const auto val = std::stoll(line.substr(start, line.size() - start));
  fish[val]++;
  // print(fish);

  for (int day = 0; day < 256; day++) {
    const long long to_add = fish[0];
    for (int i = 1; i < 7; i++) {
      fish[i-1] = fish[i];
    }
    fish[6] = to_add + fish[7];
    fish[7] = fish[8];
    fish[8] = to_add;
    // print(fish);
  }

  // std::cout << "Final numbers" << '\n';
  // print(fish);
  long long sum = 0;
  for (const auto& p : fish) {
    sum += p.second;
  }
  std::cout << sum << '\n';
  return 0;
}
