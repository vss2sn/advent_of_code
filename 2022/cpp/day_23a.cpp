#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

struct Point {
  Point (const int row = 0, const int col = 0) : row(row), col(col) {}
  int row; 
  int col;

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

struct PointHash {
  std::size_t operator () (const Point& p) const {
    return p.row * p.col;
  }
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << '(' << p.row << ' ' << p.col << ')';
  return os;
}

struct Elf {
    Point current;
    Point next;
    bool need_to_move = false;
    bool to_move = false;
};

std::array<std::array<Point, 3>, 4> move_check_order {{
    {{ Point(-1, 0), Point(-1, -1), Point(-1, 1) }}, // North
    {{ Point(1, 0),  Point(1, -1),  Point(1, 1)  }}, // South
    {{ Point(0, -1), Point(-1, -1), Point(1, -1) }}, // West
    {{ Point(0, 1),  Point(-1, 1),  Point(1, 1)  }}  // East
}};

std::array<Point, 4> moves {{
    Point(-1, 0),
    Point(1,  0),
    Point(0, -1),
    Point(0,  1)
}};

std::array<Point, 8> surroundings {{
    Point(-1, 0),
    Point(1,  0),
    Point(0, -1),
    Point(0,  1),
     Point(-1, -1),
    Point(1,  1),
    Point(1, -1),
    Point(-1,  1)
}};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_23_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Elf> elves;

  int row = 0;
  while(std::getline(file, line)) {
    int col = 0;
    for (int col = 0; col < line.size(); col++) {
        if (line[col] == '#') {
            Elf elf;
            elf.current = Point(row, col);
            elf.to_move = false;
            elves.push_back(elf);
        }
    }
    row++;
  }
  std::cout << elves.size() << '\n';
//   exit(0);
  constexpr int n_rounds = 10;
  int move_check_index = 0;
  
  for (int round = 0; round < n_rounds; round++) {
    // 1st half
    std::unordered_set<Point, PointHash> current_positions;
    std::unordered_map<Point, int, PointHash> next_positions;

    // Update current positions
    for (const auto& elf : elves) {
        current_positions.insert(elf.current);
    }

    // Check whether each elf needs to move
    for (auto& elf : elves) {
        elf.need_to_move = false;
        for (const auto& surrounding : surroundings) {
            if(current_positions.find(Point(elf.current.row + surrounding.row, elf.current.col + surrounding.col)) != current_positions.end()) {
                elf.need_to_move = true;
            } 
        }
    }

    // Check whether any elf needs to move
    if (std::all_of(std::begin(elves), std::end(elves), [](const auto elf) {return !elf.need_to_move; })) {
        round = n_rounds;
        continue;
    }

    // std::cout << "move_check_index: " << move_check_index << '\n';
    for (auto& elf : elves) {
        if (!elf.need_to_move) {
            std::cout << "Proposes not moving as does not need to move" << '\n';
            continue;
        }
        
        bool possible = true;
        for (int i = 0; i < move_check_order.size(); i++) {
            possible = true;
            for (const auto& delta : move_check_order[(move_check_index + i) % 4]) {
                const auto new_point = Point(elf.current.row + delta.row, elf.current.col + delta.col);
                if (current_positions.find(new_point) != current_positions.end()) {
                    possible = false; 
                    break;
                }
            }
            if (possible) {
                const auto selected_move = moves[(move_check_index + i) % 4];
                elf.next = Point(elf.current.row + selected_move.row, elf.current.col + selected_move.col);
                elf.to_move = true;
                std::cout << "Proposes moving " << (move_check_index + i) % 4 << '\n';
                // Avoid default init for sanity
                if (next_positions.find(elf.next) == next_positions.end()) {
                    next_positions[elf.next] = 0;
                }
                next_positions[elf.next]++;
                break;
            }
        }
        if (!possible) {
            std::cout << "Proposes not moving as cannot move " << '\n';
            elf.to_move = false;
        }
    }
    move_check_index++;
    move_check_index %= 4;

    // 2nd half
    for (auto& elf : elves) {
        if (elf.to_move && next_positions.find(elf.next)->second == 1) {
            // std::cout << elf.current << " ---> " << elf.next << '\n';
            elf.current = elf.next;
        }
        elf.to_move = false;
    }

    // Debug
    std::array<int, 4> rectangle = {{
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min(), 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    }};
    for (const auto& elf : elves) {
        rectangle[0] = std::min(rectangle[0], elf.current.row);
        rectangle[1] = std::max(rectangle[1], elf.current.row);
        rectangle[2] = std::min(rectangle[2], elf.current.col);
        rectangle[3] = std::max(rectangle[3], elf.current.col);
    }
    auto grid = std::vector<std::vector<char>>();
    for (int row = rectangle[0]; row <= rectangle[1]; row++) {
        // std::cout << row << '\n';
        grid.emplace_back();
        for (int col = rectangle[2]; col <= rectangle[3]; col++) {
            // std::cout << col << '\n';
            grid[row - rectangle[0]].emplace_back('.');
        }
    }
    for( const auto& elf : elves) {
        grid[elf.current.row - rectangle[0]][elf.current.col - rectangle[2]] = '#';
    }
    int c = 0;
    for (int row = rectangle[0]; row <= rectangle[1]; row++) {
        for (int col = rectangle[2]; col <= rectangle[3]; col++) {
            // std::cout << grid[row - rectangle[0]][col - rectangle[2]];
            if (grid[row - rectangle[0]][col - rectangle[2]] == '.') c++;
        }
        // std::cout<< '\n';
    }
    // std::cout<< '\n';
    std::cout << grid.size() << " x " << grid[0].size() << '\n';
    std::cout << c << '\n';
  } 

  return 0;
}