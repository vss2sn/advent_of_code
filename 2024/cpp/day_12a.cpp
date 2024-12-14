#include <array>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <queue>
#include <vector>

struct Point {
    int row;
    int col;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col;
    }
};

constexpr std::array<Point, 4> motions {
    Point( 0, 1),
    Point( 1, 0),
    Point( 0,-1),
    Point(-1, 0)
};

long long calculate_cost(
    const int row_idx, 
    const int col_idx, 
    const std::vector<std::string>& map,
    std::vector<std::vector<bool>>& visited) {
    const auto c = map[row_idx][col_idx];
    if (c == '#') return 0;
    const auto start = Point(row_idx, col_idx);
    long long cost = 0;
    std::queue<Point> q;
    q.push(start);
    long long area = 0;
    long long perimeter = 0;
    while(!q.empty()) {
        auto p = q.front();
        q.pop();
        if (visited[p.row][p.col]) continue;
        visited[p.row][p.col] = true;
        area++;
        for (const auto& motion : motions) {
            const auto next = Point(p.row + motion.row, p.col + motion.col);
            if (map[next.row][next.col] != c) {
                perimeter++;
                continue;
            }
            if (map[next.row][next.col] == c && !visited[next.row][next.col]) {
                q.push(next);
            }
        }
    }
    // std::cout << c << ": \nArea: " << area << "\nPerimeter: "<< perimeter << '\n';
    return area * perimeter;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_12_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> map;
    map.emplace_back();
    
    // Add a buffer
    while(std::getline(file, line)) {
        line = '#' + line + '#';
        map.push_back(line);
    }
    map[0] = std::string(map[1].size(), '#');
    map.emplace_back(map[0]);

    std::vector<std::vector<bool>> visited (map.size(), std::vector<bool>(map[0].size(), false));
    long long total_cost = 0;
    for (const auto& [row_idx, row] : std::views::enumerate(map)) {
        for (const auto& [col_idx, ele] : std::views::enumerate(row)) {
            if (visited[row_idx][col_idx]) continue;
            const auto c = map[row_idx][col_idx];
            if (c == '#') continue;
            total_cost += calculate_cost(row_idx, col_idx, map, visited);
        }
    }
    std::cout << total_cost << '\n';
    return 0;
}