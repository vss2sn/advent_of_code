#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  Point(const int row, const int col) : row(row), col(col) {}
  int row, col;

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }

  bool operator != (const Point& p) const {
    return row != p.row || col != p.col;
  }
};

struct PointHash {
  std::size_t operator() (const Point& p) const {
    return p.row * p.col;
  }
};

void print_point(const Point& p) {
  std::cout << p.row << ' ' << p.col<< '\n';
}

Point get_delta(const Point& previous, const Point& current) {
  const auto delta_row = current.row-previous.row;
  const auto delta_col = current.col-previous.col;
  return Point(
    delta_row == 0 ? 0 : delta_row / std::abs(delta_row),
    delta_col == 0 ? 0 : delta_col / std::abs(delta_col)
  );
}

Point add_points_to_set(
  const std::string& line,
  std::unordered_set<Point, PointHash>& occupied
) {
  Point lowest(0, 0);
  size_t start = 0;
  size_t end = line.find(" -> ");
  auto mid = line.find(',', start);
  auto col = std::stoi(line.substr(start, mid));
  auto row = std::stoi(line.substr(mid+1, end));
  Point previous(row, col);
  if (row > lowest.row) lowest.row = row;
  occupied.insert(previous);
  start = end + 4;
  end = line.find(" -> ", start);
  while(end != std::string::npos) {
    mid = line.find(',', start);
    col = std::stoi(line.substr(start, mid-start));
    row = std::stoi(line.substr(mid+1, end-mid-1));
    if (row > lowest.row) lowest.row = row;
    const auto current = Point(row, col);
    const auto delta = get_delta(previous, current);
    auto transition = current;
    while(transition != previous) {
      occupied.insert(transition);
      transition.row -= delta.row;
      transition.col -= delta.col;
    }
    start = end + 4;
    end = line.find(" -> ", start);
    previous = current;
  }
  mid = line.find(',', start);
  col = std::stoi(line.substr(start, mid-start));
  row = std::stoi(line.substr(mid+1, end-mid-1));
  if (row > lowest.row) lowest.row = row;
  auto current = Point(row, col);
  const auto delta = get_delta(previous, current);
  while(current != previous) {
    occupied.insert(current);
    current.row -= delta.row;
    current.col -= delta.col;
  }
  return lowest;
}

int simulate(
  std::unordered_set<Point, PointHash>& occupied,
  const Point& lowest
) {
  const auto moves = std::vector<Point> {
    {1,0},
    {1,-1},
    {1,1}
  };

  const auto start = Point(0, 500);
  int count = 0;
  while (true) {
    count++;
    Point sand(0, 500);
    bool moving = true;
    while (moving) {
      moving = false;
      for (const auto& move : moves) {
        const auto new_sand = Point(sand.row + move.row, sand.col + move.col);
        if (occupied.find(new_sand) == occupied.end()) {
          moving = true;
          sand = new_sand;
          if (new_sand.row >= lowest.row) {
            return count;
          }
          break;
        }
      }
      if (!moving) {
        occupied.insert(sand);
      }
    }
  }
  return count;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::unordered_set<Point, PointHash> occupied;

  Point lowest(0,0);
  while(std::getline(file, line)) {
    const auto new_lowest = add_points_to_set(line, occupied);
    if (new_lowest.row > lowest.row) lowest.row = new_lowest.row;
  }

  const auto count = simulate(occupied, lowest);
  std::cout << count - 1<< '\n';
  return 0;
}
