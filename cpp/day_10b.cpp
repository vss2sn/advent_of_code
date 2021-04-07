#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
  std::ifstream file{"../input/day_10_input"};
  std::vector<int> adapter_joltages;
  int adapter_joltage;
  while(file >> adapter_joltage) {
    adapter_joltages.push_back(adapter_joltage);
  }
  std::sort(std::begin(adapter_joltages), std::end(adapter_joltages));

  std::unordered_map<int, long long> cumu_combin_below; // cumulative combintaions of adapters using joltages greater than the key values
  int current_index = adapter_joltages.size() - 1;
  cumu_combin_below.insert({adapter_joltages[current_index], 1});
  while (current_index >= 0) {
    const int current_joltage = adapter_joltages[current_index];
    for(int i = 1; i <= 3; ++i) {
      const int new_joltage = current_joltage - i;
      if(cumu_combin_below.find(new_joltage) == cumu_combin_below.end()) {
        cumu_combin_below.insert({new_joltage, cumu_combin_below[current_joltage]});
      } else {
        cumu_combin_below[new_joltage] += cumu_combin_below[current_joltage];
      }
    }
    --current_index;
  }

  long long total_combinations = 0;
  for(int i = 0; i < 3; ++i) {
    if(adapter_joltages[i] <= 3) {
      total_combinations += cumu_combin_below[adapter_joltages[i]];
    } else {
      break;
    }
  }
  std::cout << total_combinations << '\n';
  return total_combinations;
}
