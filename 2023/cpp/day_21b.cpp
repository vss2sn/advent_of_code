#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert> 
#include <memory>

// This will work with both the sample input and the main input
// The core idea here is that the edges of the square grid are always empty
// This leads to a highway that can be used to reach grids 
// further away from the starting point
// At some unknown distance it becomes more efficient to 
// 1. move from the starting point to the highway
// 2. traverse the highway to the target point's grid
// 3. move from the highway to the target point
// instead of going through the grids between the start and end point
// Let this distance cover `heuristic_1` grids
// So up until that heuristic brute force is used to find the number of reachable points.
// 
// The infinite grid can be represented as 
//   ^^^^^
//   |||||
// <-CEEEC->
// <-E...E->
// <-E...E->
// <-E...E->
// <-CEEEC->
//   |||||
//   vvvvv
// where the `.`s are bruteforced 
// the edges `E` represent all the the points in the infinite grid as represented by the arrows next to them
// the corners `C` represent all the the points in the infinite grid as represented and enclosed by the arrows next to them 
// 
// Reference:
// https://github.com/jonathanpaulson/AdventOfCode/blob/master/2023/21.py

constexpr int heuristic_1 = 4;
constexpr std::size_t n_steps = 26501365;

struct Point {
  int tile_row;
  int tile_col;
  int row;
  int col;
  std::size_t step;
  std::size_t hash;

  Point(const int tile_row = 0, const int tile_col = 0, const int row = 0, const int col = 0, const int step = 0) : tile_row(tile_row), tile_col(tile_col), row(row), col(col), step(step) {
    hash = tile_row * tile_col * 10000 + row * col;
  }

  Point operator + (const Point& p) const {
    return Point (p.tile_row + tile_row, p.tile_col + tile_col, p.row + row,  p.col + col, p.step + step);
  }

  bool operator == (const Point& p) const {
    return p.tile_row == tile_row && p.tile_col == tile_col && p.row == row && p.col == col && p.step == step;
  }
  void update_hash() {
    hash = tile_row * tile_col * 10000 + row * col;
  }
};

struct hasher {
  std::size_t operator() (const Point& ps) const {
    return ps.hash;
  }
};

const std::vector<Point> motions {
  Point(0,0,-1,0,0),
  Point(0,0,0,1,0),
  Point(0,0,1,0,0),
  Point(0,0,0,-1,0),
};

// Should replace point with another struct here
std::unordered_map<Point, std::size_t, hasher> temp_map;
std::size_t helper(const std::size_t& step, const int increment, const std::size_t& n_rows) {
    std::size_t temp = (n_steps >= step) ? (n_steps - step) / n_rows : 0;
    // std::cout << temp << '\n';
    if (temp_map.find(Point(step, increment,0, 0, 0)) != temp_map.end()) {
        return temp_map[Point(step, increment, 0, 0, 0)];
    }
    std::size_t return_val = 0;
    for (std::size_t idx = 1; idx <= temp; idx++) {
        if (((step + n_rows * idx) <= n_steps) && ((step + n_rows * idx) % 2 == (n_steps % 2))) {
            return_val += (increment == 2) ? (idx + 1) : 1;
        }
    }
    temp_map[Point(step, increment, 0, 0, 0)] = return_val;
    // std::cout << return_val << '\n';
    return return_val;
}

std::unordered_map<Point, std::size_t, hasher> create_count_map(const std::vector<std::string>& map, const Point& S) {
    std::queue<Point> queue;
    std::unordered_map<Point, std::size_t, hasher> count_map;
    const auto row_size = map.size();
    const auto col_size = map[0].size();
    // std::cout << row_size << ' ' << col_size << '\n';
    // exit(0);
    queue.push(S);
    // std::cout << S.row << ' ' << S.col << '\n';
    while(!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        // std::cout << current.tile_row << ' ' 
        //             << current.tile_col << ' ' 
        //             << current.row << ' ' 
        //             << current.col << ' ' 
        //             << current.step << '\n';
        if (current.row < 0) {
            current.tile_row -= 1;
            current.row += row_size;
        }  
        if (current.row >= row_size) {
            current.tile_row += 1;
            current.row -= row_size;
        } 
        if (current.col < 0) {
            current.tile_col -= 1;
            current.col += col_size;
        } 
        if (current.col >= col_size) {
            current.tile_col += 1;
            current.col -= col_size;
        }
        // current.update_hash();
        if (current.row < 0 || current.row >= row_size || current.col < 0 || current.col >= col_size || map[current.row][current.col] == '#') continue;
        if (count_map.find(Point(current.tile_row, current.tile_col, current.row, current.col, 0)) != count_map.end()) continue;
        if (std::abs(current.tile_row) > heuristic_1 || std::abs(current.tile_col) > heuristic_1) continue;
        count_map[Point(current.tile_row, current.tile_col, current.row, current.col, 0)] = current.step;
        for (const auto& motion : motions) {
            queue.emplace(current.tile_row, current.tile_col, current.row + motion.row, current.col + motion.col, current.step + 1);
        }
    }
    return count_map;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_21_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  Point S(0,0,0,0,0);
  std::vector<std::string> map;
  while(std::getline(file, line)) {
    if (line.empty()) continue;
    map.push_back(line);
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == 'S') {
        S.row = map.size() - 1;
        S.col = i;
        S.step = 0;
      }
    }
  }

  // Count map already contains the tiles in the square contained by
  // (-heuristic_1, -heuristic_1) to (heuristic_1, 1heuristic_1)
  // inclusive of the border.
  std::vector<int> OPT;
  for (int i = -heuristic_1; i <= heuristic_1; i++){
    OPT.push_back(i);
  } 
  const auto count_map = create_count_map(map, S);
  std::size_t ans = 0;
  const auto n_rows = map.size();
  const auto n_cols = map[0].size();
  for (int row = 0; row < n_rows; row++) {
    for (int col = 0; col < n_cols; col++) {
        const auto p = Point(0, 0, row, col, 0);
        if (count_map.find(p) != count_map.end()) {            
            for (const auto tile_row : OPT) {
                for (const auto tile_col : OPT) {
                    const auto step = count_map.at(Point(tile_row, tile_col, row, col, 0));
                    // std::cout <<tile_row <<' ' << tile_col<<' ' << row<<' ' << col<<' ' << 0 << '\n';
                    if ((step % 2) == (n_steps % 2) && step <= n_steps) {
                        ans += 1;
                    }
                    if ((tile_row == -heuristic_1 || tile_row == heuristic_1) &&  (tile_col == -heuristic_1 || tile_col == heuristic_1)) {
                        ans += helper (step, 2, n_rows);
                    }
                    else if ((tile_row == -heuristic_1 || tile_row == heuristic_1) || (tile_col == -heuristic_1 || tile_col == heuristic_1)) {
                        ans += helper (step, 1, n_rows);
                    }
                }
            }
        }
    }
  }
  std::cout << ans << '\n';
  return 0;
}