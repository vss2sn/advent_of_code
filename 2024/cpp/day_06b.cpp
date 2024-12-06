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

bool explore(Point current, 
             int idx,
             std::vector<std::string>& map, 
             std::unordered_map<Point, std::array<bool, 4>, Hasher> visited_edges, 
             std::unordered_set<Point, Hasher>& obs,
             const bool obstacle_placed) {
    while(in_map(current, map)) {
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
            if (!obstacle_placed && !visited_edges.contains(next)) {
                const auto c = map[next.row][next.col];
                map[next.row][next.col] = '#'; 
                if (explore(current, (idx+1)%4, map, visited_edges, obs, true)) {
                    obs.insert(next);
                }
                map[next.row][next.col] = c; 
            }
            visited_edges[current][idx] = true;
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
        map.push_back(line);
        for (const auto [idx, c] : std::views::enumerate(line)) {
            if (c == '^') {
                start.row = map.size()-1;
                start.col = idx;
            }
        }
    }
    std::unordered_map<Point, std::array<bool, 4>, Hasher> visited_edges;
    std::unordered_set<Point, Hasher> obs;
    explore(start, 2, map, visited_edges, obs, false);
    std::cout << obs.size() << '\n';
    return 0;
} 