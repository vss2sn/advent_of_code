#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

constexpr int n_preamble = 25;

int main() {
  std::ifstream file{"../input/day_09_input"};
  int number;
  std::vector<int> preamble_v;
  std::queue<int> preamble_q;
  preamble_v.reserve(n_preamble);

  // Create preamble
  while (n_preamble > preamble_v.size() && file >> number) {
    preamble_v.push_back(number);
    preamble_q.push(number);
  }

  while (file >> number) {
    int begin = 0;
    int end = preamble_v.size() - 1;
    std::sort(std::begin(preamble_v), std::end(preamble_v));
    bool found = false;
    while (begin < end) {
      const int sum = preamble_v[begin] + preamble_v[end];
      if (sum == number && preamble_v[begin] != preamble_v[end]) {
        const auto element_to_remove = preamble_q.front();
        preamble_q.pop();
        preamble_q.push(number);
        *std::find(std::begin(preamble_v), std::end(preamble_v),
                   element_to_remove) = number;
        found = true;
        break;
      } else if (sum > number) {
        end--;
      } else if (sum < number) {
        begin++;
      }
    }
    if (!found) {
      std::cout << number << '\n';
      return number;
    }
  }
  return -1;
}
