#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

constexpr int n_preamble = 25;

int main() {
  std::ifstream file{"../input/day_09_input"};
  int number;
  std::vector<int> list;
  std::vector<int> preamble_v;
  std::queue<int> preamble_q;
  preamble_v.reserve(n_preamble);

  // Create preamble
  while (n_preamble > preamble_v.size() && file >> number) {
    preamble_v.push_back(number);
    preamble_q.push(number);
    list.push_back(number);
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
      break;
    } else {
      list.push_back(number);
    }
  }

  const int invalid_number = number;
  int begin = list.size() - 2;
  int end = list.size() - 1;
  int sum = list[begin] + list[end];
  while (begin < end && begin >= 0) {
    if (sum > invalid_number) {
      sum -= list[end];
      --end;
      if (begin == end) {
        --begin;
        sum += list[begin];
      }
    } else if (sum < invalid_number) {
      --begin;
      sum += list[begin];
    } else {
      // contiguous list found
      break;
    }
  }

  const int min_cont = *std::min_element(std::next(std::begin(list), begin),
                                         std::next(std::begin(list), end));
  const int max_cont = *std::max_element(std::next(std::begin(list), begin),
                                         std::next(std::begin(list), end));

  const int encrytion_weakness = min_cont + max_cont;
  std::cout << encrytion_weakness << '\n';
  return encrytion_weakness;
}
