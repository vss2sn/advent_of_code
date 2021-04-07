#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int main() {
  std::ifstream file{"../input/day_10_input"};
  std::vector<int> adapter_joltages;
  int adapter_joltage;
  while(file >> adapter_joltage) {
    adapter_joltages.push_back(adapter_joltage);
  }
  std::sort(std::begin(adapter_joltages), std::end(adapter_joltages));

  int n_del_1 = 0;
  int n_del_2 = 0;
  int n_del_3 = 0;
  for(int i = 0; i < adapter_joltages.size() - 1; ++i) {
    const int del = adapter_joltages[i + 1] - adapter_joltages[i];
    if(del == 1) {
      ++n_del_1;
    } else if (del == 2) {
      ++n_del_2;
    } else if (del == 3 )  {
      ++n_del_3;
    }
  }

  // Difference between lowest adapter and port
  if(adapter_joltages[0] == 1) {
    ++n_del_1;
  } else if (adapter_joltages[0] == 2) {
    ++n_del_2;
  } else if (adapter_joltages[0] == 3 )  {
    ++n_del_3;
  }

  // Difference between last adapter and device
  ++n_del_3;
  std::cout << "Product of number of adapter pairs with 1 jolt differences with the number of adapter pairs with 3 jolt differences: " << n_del_1 * n_del_3 << '\n';
  return n_del_1 * n_del_3;
}
