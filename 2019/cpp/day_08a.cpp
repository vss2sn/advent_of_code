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
  size_t index = 0;
  size_t min_index = 0;
  size_t n_zeros = pixels_in_layer;

  auto start = std::begin(image);
  while (start < std::end(image)) {
    const auto count =
        std::count_if(start, start + pixels_in_layer,
                      [](const int pixel) { return pixel == 0; });
    if (count < n_zeros) {
      n_zeros = count;
      min_index = index;
    }
    index++;
    start += pixels_in_layer;
  }

  const auto n_ones =
      std::count_if(std::begin(image) + min_index * pixels_in_layer,
                    std::begin(image) + (min_index + 1) * pixels_in_layer,
                    [](const int pixel) { return pixel == 1; });
  const auto n_twos =
      std::count_if(std::begin(image) + min_index * pixels_in_layer,
                    std::begin(image) + (min_index + 1) * pixels_in_layer,
                    [](const int pixel) { return pixel == 2; });

  const auto ans = n_ones * n_twos;
  std::cout << ans << '\n';
  return ans;
}
