#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
int main () {
  std::fstream file{"../input/day_23_input"};
  std::string input;
  std::vector<int> order;
  std::getline(file, input);
  input.erase(std::remove_if(std::begin(input), std::end(input), [](const char c) {return !isprint(c);}), std::end(input));
  for(const auto& c : input) {
    order.push_back(c - '0');
  }
  int current_index = 0;
  int round = 0;
  int n_pick_up = 3;
  int low = *std::min_element(std::begin(order), std::end(order));
  int high = *std::max_element(std::begin(order), std::end(order));
  while (round < 100) {
    int cur_val = order[current_index];
    std::vector<int> cups_picked_up;
    int delta = 0;
    for (int i = 0; i < n_pick_up; i++) {
      if (current_index + 1 < order.size()) {
        cups_picked_up.push_back(order[current_index + 1]);
        order.erase(std::begin(order) + current_index + 1);
      } else {
        cups_picked_up.push_back(order[current_index + 1 + delta - order.size()]);
        order.erase(std::begin(order) + current_index + 1 + delta - order.size());
        delta--;
      }
    }
    int to_find = cur_val - 1;
    auto it = std::find(std::begin(order), std::end(order), to_find);
    while(it == std::end(order)) {
      to_find--;
      if(to_find < low) {
        to_find = high;
      }
      it = std::find(std::begin(order), std::end(order), to_find);
    }
    int insert_index = std::distance(std::begin(order), it);
    for (int i = 0; i < n_pick_up; i++) {
      if (insert_index + i + 1 < order.size()) {
        order.insert(std::begin(order) + insert_index + i + 1, cups_picked_up[i]);
      } else {
        order.push_back(cups_picked_up[i]);
      }
    }
    round++;
    current_index = std::distance(std::begin(order), std::find(std::begin(order), std::end(order), cur_val)) + 1;
    if(current_index >= order.size()) {
      current_index = 0;
    }
  }

  std::rotate(std::begin(order), std::find(std::begin(order), std::end(order), 1), std::end(order));
  std::string ans_str = "";
  for(const auto& ele : order) {
    ans_str += std::to_string(ele);
  }
  ans_str.erase(0, 1);
  std::cout << ans_str << '\n';
  return 0;
}
