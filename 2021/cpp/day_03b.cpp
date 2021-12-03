#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int update(std::vector<bool>& fits_criteria, const std::vector<std::vector<int>>& values, const int acceptable_val, const int index, std::vector<int>& digit_sum) {
  int n_not_fit_c = 0;
  for (int i = 0; i < values.size(); i++) {
    if (values[i][index] != acceptable_val && fits_criteria[i]) {
      fits_criteria[i] = false;
      n_not_fit_c++;
      for (int j = 0; j < digit_sum.size(); j++) {
        digit_sum[j] -= values[i][j];
      }
    }
  }
  return n_not_fit_c;
}

inline int convert_to_binary(const std::vector<int>& bin_vec) {
  const int s = bin_vec.size();
  int bin_val = 0;
  for (int i = 0; i < s; i++) {
    bin_val += (bin_vec[s - i - 1]) << i;
  }
  return bin_val;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::vector<int>> values;
  while(std::getline(file, line)) {
    values.emplace_back();
    for (const auto c : line) {
      values.back().emplace_back(c - '0');
    }
  }

  const int n_values = values.size();
  const int n_digits = values[0].size();

  std::vector<int> digit_sum(n_digits);
  for (const auto& value : values) {
    for (int i = 0; i < n_digits; i++) {
      digit_sum[i] += value[i];
    }
  }

  int o2 = 0;
  int co2 = 0;
  const auto digit_sum_o = digit_sum;
  int n_fit_c = n_values;
  std::vector<bool> fits_criteria(n_values, true);
  for (int index = 0; index < n_digits; index ++) {
    if (digit_sum[index] * 2 >= n_fit_c) {
      n_fit_c -= update(fits_criteria, values, 1, index, digit_sum);
    } else {
      n_fit_c -= update(fits_criteria, values, 0, index, digit_sum);
    }
    if (n_fit_c == 1) {
      const auto o2_index = std::distance(std::begin(fits_criteria), std::find(std::begin(fits_criteria), std::end(fits_criteria), true));
      o2 = convert_to_binary(values[o2_index]);
      break;
    }
  }

  n_fit_c = n_values;
  fits_criteria = std::vector<bool>(n_values, true);
  digit_sum = digit_sum_o;
  for (int index = 0; index < n_digits; index ++) {
    if (digit_sum[index] * 2 >= n_fit_c) {
      n_fit_c -= update(fits_criteria, values, 0, index, digit_sum);
    } else {
      n_fit_c -= update(fits_criteria, values, 1, index, digit_sum);
    }
    if (n_fit_c == 1) {
      const auto co2_index = std::distance(std::begin(fits_criteria), std::find(std::begin(fits_criteria), std::end(fits_criteria), true));
      co2 = convert_to_binary(values[co2_index]);
      break;
    }
  }

  std::cout << o2 * co2 << '\n';

  return 0;
}
