#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <unordered_set>
#include <vector> 

struct Point {
    Point(const int row = 0, const int col = 0) : row(row), col(col) {}
    int row;
    int col;
    bool operator == (const Point& p) const {
        return p.row == row && p.col == col;
    }

};

struct Hasher {
    std::size_t operator () (const Point& p) const {
        return p.row;
    }
};

const std::vector<Point> moves {
    Point(1,0),
    Point(0,-1),
    Point(-1,0),
    Point(0,1),
};

const bool in_map (const Point& p, const std::vector<std::string>& map) {
    return  !(p.col >= map[0].size() || p.col < 0 || p.row >= map.size() || p.row < 0);
}


int main(int argc, char* argv[]) {
    std::string input = "../input/day_06_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> map;
    Point start;
    while(std::getline(file, line)) {
        // std::cout << line << '\n';
        map.push_back(line);
        for (const auto [idx, c] : std::views::enumerate(line)) {
            if (c == '^') {
                start.row = map.size()-1;
                start.col = idx;
            }
        }
    }
    std::unordered_set<Point, Hasher> visited;
    auto current = start;
    visited.insert(current);
    int idx = 2;
    while(true) {
        const auto next = Point(current.row + moves[idx].row, current.col + moves[idx].col);
        if (!in_map(next, map)) break;
        if (map[next.row][next.col] != '#') {
            visited.insert(next);
            // map[current.row][current.col] = 'X';
            // map[next.row][next.col] = '*';
            current = next;
        } else {
            idx += 1;
            idx %= 4;
        }
    }
    std::cout << visited.size() << '\n';
    return 0;
}