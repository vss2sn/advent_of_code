#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

constexpr int row_chars = 7;
constexpr int col_chars = 3;
constexpr int n_rows = std::pow(2, row_chars);
constexpr int n_cols = std::pow(2, col_chars);
constexpr int first_row = 0;
constexpr int first_col = 0;

int convertSeatBSPToSeatID(const std::string& seat_bsp) {
  double current_row_max = n_rows;
  double current_row_min = first_row;
  double current_col_max = n_cols;
  double current_col_min = first_col;

  for(int i = 0; i < row_chars; ++i) {
    if(seat_bsp[i] == 'F') {
      current_row_max = (current_row_max + current_row_min) / 2;
    } else {
      current_row_min = (current_row_max + current_row_min) / 2;
    }
  }
  for(int i = row_chars; i < (row_chars + col_chars); ++i) {
    if(seat_bsp[i] == 'L') {
      current_col_max = (current_col_max + current_col_min) / 2;
    } else {
      current_col_min = (current_col_max + current_col_min) / 2;
    }
  }
  return (current_row_min) * n_cols  + (current_col_min);
}

int main() {
  std::ifstream file;
  file.open("../input/day_5_input");

  std::string seat_bsp;
  int high_seat = 0;
  while(std::getline(file, seat_bsp)) {
    high_seat = std::max(convertSeatBSPToSeatID(seat_bsp), high_seat); // Human readable seat
  }

  std::cout << high_seat << '\n';
  return high_seat;
}
