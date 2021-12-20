#include <array> 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print(const std::vector<std::vector<int>>& v) {
  for (const auto& row : v) {
    for (const auto ele : row) {
      std::cout << (ele == 1 ? '#' : '.') ;
    }
    std::cout << '\n';
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

  std::vector<std::vector<int>> image_input;

  std::getline(file, line);
  std::vector<char> algo;
  for (const auto c : line) {
    algo.emplace_back(c == '#' ? 1 : 0);
  }
  std::getline(file, line);

  while(std::getline(file, line)) {
    if (line == "") continue;
    image_input.emplace_back();
    for (const int c : line) {
      image_input.back().push_back(c == '#' ? 1 : 0);
    }
  }
  // print(image_input);

  constexpr int n_steps = 50;
  constexpr std::array<int, 9> pows_of_2 = {{1, 2, 4, 8, 16, 32, 64, 128, 256}};

  // The grid is 3x3 so each pixel in the input can affect a pixel 2 positions away from it
  // Also need to account for that in both + and - directions for ech axis
  // Hence 2*2
  std::vector<std::vector<std::vector<int>>> images(n_steps + 1,
    std::vector<std::vector<int>>(2 * 2 * (n_steps) + image_input.size(),
      std::vector<int>(2 * 2 * (n_steps) + image_input[0].size(), 0)
    )
  );

  for (int i = 0; i < image_input.size(); i++) {
    for (int j = 0; j < image_input[0].size(); j++) {
      images[0][2 * n_steps + i][2 * n_steps + j] = image_input[i][j];
    }
  }

  const std::vector<std::vector<int>> deltas = {
    {-1,-1},
    {-1,0},
    {-1,1},
    {0,-1},
    {0,0},
    {0,1},
    {1,-1},
    {1,0},
    {1,1}
  };

  const auto in_bounds = [&n_steps, &images](const int step, const int row, const int col) {
    return row >= (n_steps - step) && col >= (n_steps - step) && row < images[step].size() - (n_steps - step) && col < images[step][0].size() - (n_steps - step);
  };

  const auto convert_to_decimal = [&images, &pows_of_2, &algo, &in_bounds, &deltas](const int step, const int row, const int col) {
    auto sum = 0;
    for (int i = 0; i < 9; i++) {
      if (in_bounds(step, row + deltas[i][0], col + deltas[i][1])) {
        sum += (images[step][row+deltas[i][0]][col+deltas[i][1]] * pows_of_2[8-i]);
      } else if (algo[0] == 0) { // Account for rest of infinite image not in input
        // do nothing as none of the ones in the rest of the image light up,
        // so the rest of the image is always 0 until the image input propogates out into it
      } else if (step % 2 == 0) { // Account for rest of infinite image not in input
        sum += algo[511] * pows_of_2[8-i];
      } else { // Account for rest of infinite image not in input
        sum += algo[0] * pows_of_2[8-i];
      }
    }
    return sum;
  };

  for (int step = 0; step < n_steps; step++) {
    for (int i = 0; i < images[0].size(); i++) {
      for (int j = 0; j < images[0][0].size(); j++) {
        images[step+1][i][j] = algo[convert_to_decimal(step, i, j)];
      }
    }
    // print(images[step+1]);
  }

  int count = 0;
  for (int i = 1; i < images[0].size() - 1; i++) {
    for (int j = 1; j < images[0].size()-1; j++) {
      count += images[n_steps][i][j];
    }
  }
  std::cout << count << '\n';
  return 0;
}
