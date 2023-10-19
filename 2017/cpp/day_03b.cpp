#include <fstream>
#include <iostream>
#include <string>
#include <charconv>
#include <limits>
#include <ranges>
#include <vector> 

// #include <chrono>
// #include <thread>

// using namespace std::this_thread; // sleep_for, sleep_until
// using namespace std::chrono; // nanoseconds, srowstem_clock, seconds

void update_grid(std::vector<std::vector<int>>& grid, const int col, const int row) {
  int sum = 0;
  for (int i = -1; i <=1; i++) {
    for (int j = -1; j <=1; j++) {
      if (row+j < 0 || col+i < 0 ) {
        continue;
      }
      sum += grid[row+j][col+i];
    }
  }
  grid[row][col] = sum;
}

void print_grid(const std::vector<std::vector<int>>& grid) {
  for (int row = grid.size()-1; row>=0; row--){
    for (int col = 0; col < grid[0].size(); col++) {
      std::cout << grid[row][col] << ' ';
    }
    std::cout << '\n';
  }
}

bool check_if_left_empty(const std::vector<std::vector<int>>& grid, const int dir, const int col, const int row) {
  const int new_dir = (dir + 1) % 4;
  if (new_dir == 0) {
    // std::cout << "Checking if " << row+1 << " " << col << " is empty" << '\n';
    // std::cout << "it's value is: " << grid[row+1][col] << '\n';
    return (grid[row+1][col] == 0);
  } else if (new_dir == 1) {
    // std::cout << "Checking if " << row << " " << col-1 << " is empty" << '\n';
    // std::cout << "it's value is: " << grid[row][col-1] << '\n';
    return (grid[row][col-1] == 0);
  } else if (new_dir == 2) {
    // std::cout << "Checking if " << row-1 << " " << col << " is empty" << '\n';
    // std::cout << "it's value is: " << grid[row-1][col] << '\n';
    return (grid[row-1][col] == 0);
  } else if (new_dir == 3) {
    // std::cout << "Checking if " << row << " " << col+1 << " is empty" << '\n';
    // std::cout << "it's value is: " << grid[row][col+1] << '\n';
    return (grid[row][col+1] == 0);
  }
  return false;
}


int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_03_input" ;  
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  int number = std::stoi(line);
  int box_side = 1;
  if (number == 1) {
    std::cout << 2 << '\n';
    return 0;
  }
  while (number >= (box_side * box_side) * 2) {
    box_side += 2;
  }
  // std::cout << "number: " << number << '\n';
  // std::cout << "box_side: " << box_side << '\n';
  
  int distance = 0;
  const auto box_side_2 = box_side * box_side;
  std::vector<std::vector<int>> grid (box_side + 2, std::vector<int>(box_side + 2, 0));
  // std::cout << grid.size() << '\n';
  int col = box_side/2 + 1;
  int row = box_side/2 + 1;
  grid[row][col] = 1;
  // std::cout << row << ' ' << col << '\n';
  col++;
  grid[row][col] = 1;
  int dir = 3;
  while(number >= grid[row][col]) {
    // print_grid(grid);
    // std::cout << "Current point: " << row << ' ' << col << '\n';
    // std::cout << "grid: " << grid[row][col] << '\n';
    // std::cout << "dir: " << dir << '\n';
    while(!check_if_left_empty(grid, dir, col, row)) {
      // print_grid(grid);  
      if (dir == 0) {
        row++;
      } 
      else if (dir == 1) {
        col--;
      }
      else if (dir == 2) {
        row--;
      }
      else if (dir == 3) {
        col++;
      }
      update_grid(grid, col, row);
      // std::cout << __LINE__ << '\n';
      // std::cout << "(" << row << ", " << col << "): " << grid[row][col] << '\n';
      if (grid[row][col] > number) {
        // print_grid(grid);
        std::cout << grid[row][col] << '\n';
        // std::cout << col + row - 2 * (box_side/2 + 1) << '\n';
        return 0;
      }
      // std::cout << __LINE__ << '\n';
    }
    dir++;
    dir %= 4;
    // std::cout << __LINE__ << '\n';
    // sleep_for(seconds(1));

  }
  // std::cout << __LINE__ << '\n';
  return 0;
}