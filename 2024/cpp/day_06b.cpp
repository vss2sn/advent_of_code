#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector> 

struct Point {
    Point(const int row = 0, const int col = 0) : row(row), col(col) {}
    int row;
    int col;
    bool operator == (const Point& p) const {
        return p.row == row && p.col == col;
    }
    bool operator < (const Point& p) const {
        return p.row < row || (p.row == row && p.col < col);
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
std::unordered_set<Point, Hasher> visited;
bool explore(Point current, 
             int idx,
             std::vector<std::string> map, 
             std::unordered_map<Point, std::array<bool, 4>, Hasher> visited_edges, 
             std::unordered_set<Point, Hasher>& obs,
             const bool obstacle_placed) {
    while(in_map(current, map)) {
        if (!obstacle_placed) visited.insert(current);
        if (!visited_edges.contains(current)) {
            visited_edges[current] = {{false, false, false, false}};    
        }
        if (visited_edges[current][idx] && obstacle_placed) {
            return true;
        }    
        const auto next = Point(current.row + moves[idx].row, current.col + moves[idx].col);        
        if (!in_map(next, map)) return false;
        if(map[next.row][next.col] == '#') {    
            idx += 1;
            idx %= 4;
        } else {
            // if (!obstacle_placed) {
            //     const auto c = map[next.row][next.col];
            //     map[next.row][next.col] = '#'; 
            //     auto next_idx = (idx+1)%4;
            //     if (explore(current, next_idx, map, visited_edges, obs, true)) {
            //         map[next.row][next.col] = 'O';
            //         for (const auto& line : map) {
            //             std::cout << line << '\n';
            //         }
            //         std::cout << '\n';
            //         obs.insert(next);
            //     }
            //     map[next.row][next.col] = c; 
            // }
            visited_edges[current][idx] = true;
            map[current.row][current.col] = 'X';
            map[next.row][next.col] = '*';
            current = next;
        }
    }
    return false;
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
    Point current = start;
    int idx = 2;
    std::unordered_map<Point, std::array<bool, 4>, Hasher> visited_edges;
    std::unordered_set<Point, Hasher> obs;
    explore(current, idx, map, visited_edges, obs, false);
    int ans = 0;
    for (const auto& v : visited) {
        map[v.row][v.col] = '#';
        if(explore(current, idx, map, visited_edges, obs, true)) ans++;
        map[v.row][v.col] = '.';
    }
    std::cout << obs.size() << '\n';
    std::cout << visited.size() << '\n';
    std::cout << ans << '\n';
    return 0;
}