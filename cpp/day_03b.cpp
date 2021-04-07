#include <fstream>
// #include <iostream>
#include <vector>

int main() {
  std::ifstream file;
  file.open("../input/day_3_input");
  std::string row;
  std::vector<std::string> map;
  std::vector<std::string> slopes =
    {
      {1, 1},
      {3, 1},
      {5, 1},
      {7, 1},
      {1, 2}
    };
  while(file >> row) {
    map.push_back(row);
  }
  const int n_rows = map.size();
  if(n_rows == 0) return 0;

  const int n_cols = map[0].size();
  long long product = 1;

  for (const auto& slope : slopes) {
    const int inc_x = slope[0], inc_y = slope[1];
    if(slope[0] > n_cols || slope[1] > n_rows) continue;
    int position_x = 0, position_y = 0;
    int n_tree = 0;
    while(position_y < n_rows) {
      if(map[position_y][position_x] == '#') {
        ++n_tree;
      }
      position_x += inc_x;
      position_x %= n_cols;
      position_y += inc_y;
    }
    // std::cout << n_tree << '\n';
    product *= n_tree;
  }
  // std::cout << product << '\n';
  return product;
}
