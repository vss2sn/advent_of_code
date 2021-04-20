#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  std::vector<int> image;
  image.reserve(line.size());
  for (const auto c : line) {
    image.emplace_back(c - '0');
  }

  // Solve
  constexpr int n_rows = 6;
  constexpr int n_cols = 25;
  constexpr int pixels_in_layer = n_rows * n_cols;
  std::array<int, pixels_in_layer> final_image;
  size_t index = 0;
  size_t min_index = 0;
  size_t n_zeros = pixels_in_layer;

  std::fill(std::begin(final_image), std::end(final_image), 2);
  for (int i = 0; i < pixels_in_layer; i++) {
    auto it = std::begin(image) + i;
    while (it < std::end(image)) {
      if (*it != 2) {
        final_image[i] = *it;
        break;
      }
      it += pixels_in_layer;
    }
  }

  for (int i = 0; i < n_rows; i++) {
    for (int j = 0; j < n_cols; j++) {
      std::cout << final_image[i * n_cols + j] << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}
