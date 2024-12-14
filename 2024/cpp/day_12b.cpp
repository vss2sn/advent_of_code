#include <array>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <queue>
#include <vector>

// Number of edges == number of corners

struct Point {
    int row;
    int col;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col;
    }
};

// This order matters
constexpr std::array<Point, 4> motions {
    Point( 0,-1),
    Point(-1, 0),
    Point( 0, 1),
    Point( 1, 0)
};

int check_corner(const Point& p, const std::vector<std::string>& map) {
    int nc = 0;
    const auto c = map[p.row][p.col];

    // Check a 2x2 square with the point p at top left, top right, bottom left and bottom right
    // When point p is top left, the possibilities are:
    // |..|..|.X|.X|..|.X|
    // |..|.X|.X|X.|XX|XX|
    // |NC|IC|NC|IC|NC|OC|
    // NC == not a corner
    // IC == inside corner
    // OC == outside corner
    // Rotate the above for the other cases (p in top right, etc)
    // the combined result of the 4 possibilities (rotations) == checking the 3x3 matrix with p at the center

    // Outside corners
    if (map[p.row+1][p.col] != c && map[p.row][p.col+1] != c) nc++; // p in the top left position
    if (map[p.row+1][p.col] != c && map[p.row][p.col-1] != c) nc++; // p in the top right position
    if (map[p.row-1][p.col] != c && map[p.row][p.col+1] != c) nc++; // p in the bottom left position
    if (map[p.row-1][p.col] != c && map[p.row][p.col-1] != c) nc++; // p is the bottom right position
    
    // Inside corners
    if (map[p.row+1][p.col] == c && map[p.row][p.col+1] == c && map[p.row+1][p.col+1] != c) nc++; // p in the top left position
    if (map[p.row+1][p.col] == c && map[p.row][p.col-1] == c && map[p.row+1][p.col-1] != c) nc++; // p in the top right position
    if (map[p.row-1][p.col] == c && map[p.row][p.col+1] == c && map[p.row-1][p.col+1] != c) nc++; // p in the bottom left position
    if (map[p.row-1][p.col] == c && map[p.row][p.col-1] == c && map[p.row-1][p.col-1] != c) nc++; // p is the bottom right position
    
    return nc;
}

long long calculate_cost(
    const int row_idx, 
    const int col_idx, 
    const std::vector<std::string>& map,
    std::vector<std::vector<bool>>& visited) {
    const auto c = map[row_idx][col_idx];
    const auto start = Point(row_idx, col_idx);
    long long cost = 0;
    std::queue<Point> q;
    q.push(start);
    long long area = 0;
    long long corner = 0;
    while(!q.empty()) {
        auto p = q.front();
        q.pop();
        if (visited[p.row][p.col]) continue;
        visited[p.row][p.col] = true;
        area++;
        corner += check_corner(p, map);
        for (const auto& motion : motions) {
            const auto next = Point(p.row + motion.row, p.col + motion.col);
            if (map[next.row][next.col] != c) {
                continue;
            }
            if (map[next.row][next.col] == c && !visited[next.row][next.col]) {
                q.push(next);
            }
        }
    }
    // std::cout << c << ": \nArea: " << area << "\nCorners: "<< corners << '\n';
    return area * corner;
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