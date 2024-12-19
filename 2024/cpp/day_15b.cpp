#include <array>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector> 

struct Point {
    int row;
    int col;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col;
    }
    
    bool operator < (const Point& p) const {
        return row < p.row || (row == p.row &&  col < p.col);
    }
};

struct Hasher {
    std::size_t operator () (const Point& p) const {
        return p.row;
    }
};

bool in_map(const Point& p, const std::vector<std::string>& map ) {
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[0].size();
}

bool attempt_move_obstacles(const Point& current, const Point& possible_move, std::vector<std::string>& map) {
    if(map[current.row][current.col] == '.') return true;
    if(map[current.row][current.col] == '#') return false;
    if(map[current.row][current.col] == '[') {
        const bool possible_1 = attempt_move_obstacles(Point(current.row + possible_move.row, current.col + possible_move.col), possible_move, map);
        bool possible_2 = true;
        if (possible_move.row != 0) possible_2 = attempt_move_obstacles(Point(current.row + possible_move.row, current.col + 1), possible_move, map);
        return possible_1 && possible_2;
    }
    if(map[current.row][current.col] == ']') {
        const bool possible_1 = attempt_move_obstacles(Point(current.row + possible_move.row, current.col + possible_move.col), possible_move, map);
        bool possible_2 = true;
        if (possible_move.row != 0) possible_2 = attempt_move_obstacles(Point(current.row + possible_move.row, current.col - 1), possible_move, map);
        return possible_1 && possible_2;
    }
    return false;
}

void move_obstacles(const Point& current, const Point& possible_move, std::vector<std::string>& map) {
    if(map[current.row][current.col] == '.') return;
    if (map[current.row][current.col] == '[') {
        move_obstacles(Point(current.row + possible_move.row, current.col + possible_move.col), possible_move, map);
        std::swap(map[current.row][current.col], map[current.row+possible_move.row][current.col+possible_move.col]);
        if (possible_move.row != 0 && map[current.row][current.col+1] == ']') move_obstacles(Point(current.row, current.col + 1), possible_move, map);
    }
    else if (map[current.row][current.col] == ']') {
        move_obstacles(Point(current.row + possible_move.row, current.col + possible_move.col), possible_move, map);
        std::swap(map[current.row][current.col], map[current.row+possible_move.row][current.col+possible_move.col]);
        if (possible_move.row != 0 && map[current.row][current.col-1] == '[') move_obstacles(Point(current.row, current.col - 1), possible_move, map);
    }    
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_15_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> map;
    Point start;
    while(std::getline(file, line) && !line.empty()) {
        std::string updated_line = "";
        for (const auto ele : line) {
            if (ele == '#') updated_line += "##";
            else if (ele == '.') updated_line += "..";
            else if (ele == '@') updated_line += "@.";
            else if (ele == 'O') updated_line += "[]";
        }
        map.push_back(updated_line);
        for (const auto [idx,ele] : std::views::enumerate(updated_line)) {
            if (ele == '@') {
                start.row = map.size()-1;
                start.col = idx;
            }
        }
    }
    std::string moves;
    while(std::getline(file, line)) {
        moves += line;
    }

    Point current = start;
    std::array<Point, 4> possible_moves {{
        Point(-1,0),
        Point(0,1),
        Point(1,0),
        Point(0,-1)
    }};
    for (const auto move : moves) {
        // for (const auto& row : map) {
        //     std::cout << row << '\n';
        // }
        // std::cout << move << '\n';
        int move_idx = 0;
        if (move == '^') {
            move_idx = 0;
        } else if (move == '>') {
            move_idx = 1;
        } else if (move == 'v') {
            move_idx = 2;
        } else if (move == '<') {
            move_idx = 3;
        }
        Point new_point;
        new_point.row = current.row + possible_moves[move_idx].row; 
        new_point.col = current.col + possible_moves[move_idx].col;
        if (!in_map(new_point, map)) {
            continue;
        }
        if (map[new_point.row][new_point.col] == '#') {
            continue;
        }
        if (map[new_point.row][new_point.col] == '.') {
            std::swap(map[new_point.row][new_point.col], map[current.row][current.col]);
            current = new_point;
        }
        if (map[new_point.row][new_point.col] == '[' || map[new_point.row][new_point.col] == ']') {
            if (attempt_move_obstacles(new_point, possible_moves[move_idx], map)) {
                move_obstacles(new_point, possible_moves[move_idx], map);
                std::swap(map[current.row][current.col], map[new_point.row][new_point.col]);
                current = new_point;
            }
        }
    }
    int total = 0;
    for (const auto& [row_idx, row] : std::views::enumerate(map)) {
        for (const auto& [col_idx, ele] : std::views::enumerate(row)) {
            if (ele == '[') {
                total += (100 * row_idx + col_idx);
            }
        }
    }
    std::cout << total << '\n';
    return 0;
}