#include <array>
#include <fstream>
#include <iostream>
#include <queue> 
#include <ranges>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility> 
#include <vector> 

// constexpr int map_size = 7;

constexpr int map_size = 71;

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


constexpr std::array<Point, 4> moves {
    Point( 0, 1),
    Point( 1, 0),
    Point( 0,-1),
    Point(-1, 0)
};

struct Hasher {
    std::size_t operator () (const Point& p) const {
        return p.row;
    }
};

bool in_map(const Point& p, const std::array<std::array<bool, map_size>, map_size>& map ) {
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[0].size();
}

struct Comparator {
    bool operator()(
        const std::tuple<Point, Point, int>& p1, 
        const std::tuple<Point, Point, int>& p2) const {
        return std::get<2>(p1) > std::get<2>(p2);
    }
};

bool search(const std::array<std::array<bool, map_size>, map_size>& map, const Point& start, const Point& end) {
    std::priority_queue<std::tuple<Point, Point, int>, std::vector<std::tuple<Point, Point, int>>, Comparator> pq;
    std::unordered_map<Point, Point, Hasher> parent_map;
    pq.push({start, start, 0});
    while(!pq.empty()){
        const auto [current, parent, score] = pq.top();
        pq.pop();
        if (parent_map.contains(current)) continue;
        parent_map[current] = parent;
        if (current.row == end.row && current.col == end.col) {
            return true;
        }
        for (const auto& move : moves) {
            Point new_point;
            new_point.row = current.row + move.row; 
            new_point.col = current.col + move.col;            
            if (!in_map(new_point, map)) {
                continue;
            } else if (!map[new_point.row][new_point.col]) {
                continue;
            } else {
                pq.push({new_point, current, score+1});
            }
        }
    }
    return false;
}  

int main(int argc, char* argv[]) {
    std::string input = "../input/day_18_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::array<std::array<bool, map_size>, map_size> map;
    for (auto& row : map) {
        for (auto& ele : row) {
            ele = true;
        }
    }
    Point start (0,0);
    Point end (map_size-1, map_size-1);
    std::vector<Point> corrupt_data;
    while(std::getline(file, line)) {
        std::size_t comma_idx = line.find(',');
        corrupt_data.emplace_back(std::stoi(line.substr(0, comma_idx)), std::stoi(line.substr(comma_idx+1, line.size() - comma_idx - 1)));
    }
    for (const auto p : corrupt_data) {
        map[p.row][p.col] = false;
        if(!search(map, start, end)) {
            std::cout << p.row << ',' << p.col << '\n';
            break;
        }
    }

    return 0;
}