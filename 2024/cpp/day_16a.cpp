#include <array>
#include <fstream>
#include <iostream>
#include <queue> 
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility> 
#include <vector> 

struct Point {
    int row;
    int col;
    int dir;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col && dir == p.dir;
    }
    
    bool operator < (const Point& p) const {
        return row < p.row || (row == p.row &&  col < p.col)  || (row == p.row &&  col == p.col && dir < p.dir);
    }
};


constexpr std::array<Point, 4> moves {
    Point( 0, 1, 0),
    Point( 1, 0, 1),
    Point( 0,-1, 2),
    Point(-1, 0, 3)
};

struct Hasher {
    std::size_t operator () (const Point& p) const {
        return p.row;
    }
};

bool in_map(const Point& p, const std::vector<std::string>& map ) {
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[0].size();
}

struct Comparator {
    bool operator()(const std::pair<Point, int>& p1, const std::pair<Point, int>& p2) const {
        return p1.second > p2.second;
    }
};

int main(int argc, char* argv[]) {
    std::string input = "../input/day_16_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> map;
    Point start;
    Point end;
    while(std::getline(file, line)) {
        map.push_back(line);
        for (const auto [idx,ele] : std::views::enumerate(line)) {
            if (ele == 'S') {
                start.row = map.size()-1;
                start.col = idx;
                start.dir = 0;
            }
            else if (ele == 'E') {
                end.row = map.size()-1;
                end.col = idx;
            }
        }
    }
    

    std::priority_queue<std::pair<Point, int>, std::vector<std::pair<Point, int>>, Comparator> pq;
    std::unordered_set<Point, Hasher> visited;
    pq.push({start, 0});
    while(!pq.empty()){
        const auto [current, score] = pq.top();
        pq.pop();
        if (visited.contains(current)) continue;
        visited.insert(current);
        if (current.row == end.row && current.col == end.col) {
            std::cout << score << '\n';
            return 0;
        }
        for (const auto& move : moves) {
            Point new_point;
            new_point.row = current.row + move.row; 
            new_point.col = current.col + move.col;
            new_point.dir = move.dir;
            
            if (!in_map(new_point, map)) {
                continue;
            }
            else if (map[new_point.row][new_point.col] == '#') {
                continue;
            }
            else { //(map[new_point.row][new_point.col] == '.') {
                if (current.dir == new_point.dir) {
                    pq.push({new_point, score + 1});
                } else if (std::abs(current.dir - new_point.dir) == 2) {
                    pq.push({new_point, score + 2001});
                } else {
                    pq.push({new_point, score + 1001});
                }
            }
        }
    }
    std::cout << "No solution found" << '\n';
    return 0;
}