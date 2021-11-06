#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {

  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  long long n_2_reps = 0;
  long long n_3_reps = 0;

  std::fstream file(input);
  while (std::getline(file, line)) {
    std::vector<int> counts(26, 0);
    for (const auto c : line) {
      counts[c - 'a']++;
    }
    if(std::find(std::begin(counts), std::end(counts), 2) != std::end(counts)) {
      n_2_reps++;
    }
    if(std::find(std::begin(counts), std::end(counts), 3) != std::end(counts)) {
      n_3_reps++;
    }
  }
  const long long ans = n_2_reps * n_3_reps;
  std::cout << ans << '\n';
  return ans;
}
