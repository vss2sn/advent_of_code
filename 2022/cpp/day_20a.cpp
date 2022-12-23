#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print(std::vector<int> prev, std::vector<int> next, std::vector<int> init_values, int start) {
  std::cout << "Order: ";
  int count = 0;
  int index = start;
  while(count < init_values.size()) {
    std::cout << init_values[index] << ' ';
    index = next[index];
    count++;
  }
  std::cout << '\n';
  index = prev[start];
  std::vector<int> temp;
  count = 0;
  std::cout << "Order: ";
  while(count < init_values.size()) {
    temp.push_back(init_values[index]);
    index = prev[index];
    count++;
  }
  std::reverse(std::begin(temp), std::end(temp));
  for (const auto ele : temp) std::cout << ele << ' ';
  std::cout << '\n';
}

void print(std::vector<int> values) {
  for (const auto& ele : values) {
    std::cout << ele << ' ';
  }
  std::cout << '\n';
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_20_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);

  std::vector<int> init_values;
  while(std::getline(file, line)) {
    init_values.push_back(std::stoi(line));
  }

  std::vector<int> next(init_values.size());
  std::vector<int> prev(init_values.size());
  for (int i = 0; i < init_values.size(); i++) {
    next[i] = i + 1;
    prev[i] = i - 1;
  }
  prev[0] = prev.size() - 1;
  next[next.size() - 1] = 0;
  int start  = 0;
  // print(prev, next, init_values, start);

  for (int i = 0; i < init_values.size(); i++) {
    // std::cout << "Moving value: " << init_values[i] << '\n';
    int delta = init_values[i];
    int dd = (delta == 0) ? 0 : delta/std::abs(delta);
    int current = i;
    if (i == start ) {
      if (init_values[start] != 0) {
        start = next[start];
      }
    }
    if (dd > 0) {
      delta = delta % (init_values.size()-1);
      next[prev[i]] = next[i];
      prev[next[i]] = prev[i];
      while (delta != 0) {
        // std::cout << init_values[i] << " jumps in front of " << init_values[next[current]] << '\n';
        current = next[current];
        delta -= dd;
      }
      next[i] = next[current];
      prev[next[current]] = i;
      prev[i] = current;
      next[current] = i;
      // std::cout << init_values[i] << " is now prev to "<< init_values[next[current]] << '\n';
      // std::cout << init_values[current] << " is now prev to "<< init_values[i] << '\n';
    } else if (dd < 0) {
      delta = -(std::abs(delta) % (init_values.size() - 1));
      next[prev[i]] = next[i];
      prev[next[i]] = prev[i];

      while (delta != 0) {
        // std::cout << init_values[i] << " jumps behind " << init_values[prev[current]] << '\n';
        current = prev[current];
        delta -= dd;
      }

      next[i] = current;
      next[prev[current]] = i;
      prev[i] = prev[current];
      prev[current] = i;
    }
    // print(prev, next, init_values, start);
    // std::cout << '\n';
  }
  // print(prev, next, init_values, start);

  auto temp = std::vector<int>();
  int count = 0;
  int index = start;
  while(count < init_values.size()) {
    temp.push_back(init_values[index]);
    index = next[index];
    count++;
  }
  // std::cout << (temp.size() == init_values.size()) << '\n';
  int ans = 0;
  // print(temp);
  auto index_0 = std::find(std::begin(temp), std::end(temp), 0) - std::begin(temp);
  // std::cout << index_0 << '\n';
  index = (1000 % init_values.size() + index_0) % init_values.size();
  // std::cout << temp[index] << ' ';
  ans += temp[index];
  index = (2000 % init_values.size() + index_0) % init_values.size();
  // std::cout << temp[index] << ' ';
  ans += temp[index];
  index = (3000 % init_values.size() + index_0) % init_values.size();
  // std::cout << temp[index] << ' ';
  ans += temp[index];
  // std::cout << '\n';
  std::cout << ans << '\n';
}
