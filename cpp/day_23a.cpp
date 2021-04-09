#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr int n_cups = 9;
constexpr int n_rounds = 100;

int main() {
  std::fstream file{"../input/day_23_input"};
  std::string input;
  std::vector<int> order(n_cups + 1);
  std::vector<int> input_v;
  std::getline(file, input);
  input.erase(std::remove_if(std::begin(input), std::end(input),
                             [](const char c) { return !isprint(c); }),
              std::end(input));
  for (int i = 0; i < input.size(); i++) {
    input_v.push_back(input[i] - '0');
  }
  for (int i = 0; i < input_v.size() - 1; i++) {
    order[input_v[i]] = input_v[i + 1];
  }
  order[input_v[input_v.size() - 1]] = input_v[0];
  int cur_val = input_v[0];
  int round = 0;
  int n_pick_up = 3;
  int low = *std::min_element(std::begin(input_v), std::end(input_v));
  int high = *std::max_element(std::begin(order), std::end(order));
  while (round < n_rounds) {
    std::vector<int> cups_picked_up;
    int picked_val = order[cur_val];
    for (int i = 0; i < n_pick_up; i++) {
      cups_picked_up.push_back(picked_val);
      picked_val = order[picked_val];
    }
    order[cur_val] = order[cups_picked_up.back()];
    int to_find = cur_val - 1;
    if (to_find < low) {
      to_find = high;
    }
    auto it = std::find(std::begin(cups_picked_up), std::end(cups_picked_up),
                        to_find);
    while (it != std::end(cups_picked_up)) {
      to_find--;
      if (to_find < low) {
        to_find = high;
      }
      it = std::find(std::begin(cups_picked_up), std::end(cups_picked_up),
                     to_find);
    }
    auto temp = order[to_find];
    order[to_find] = cups_picked_up[0];
    order[cups_picked_up.back()] = temp;
    round++;
    cur_val = order[cur_val];
  }
  std::string ans_str = "";
  size_t temp = 1;
  for (int i = 0; i < n_cups - 1; ++i) {
    temp = order[temp];
    ans_str += std::to_string(temp);
  }
  std::cout << ans_str << '\n';
  return 0;
}
