#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <unordered_set>
#include <unordered_map>
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

constexpr std::array<Point, 4> moves {
    Point(1,0),
    Point(0,1),
    Point(-1,0),
    Point(0,-1)
    
};

void search(const Point& p, const std::vector<std::string>& map, int& count) {
    const auto value = map[p.row][p.col];
    for (const auto& move : moves) {
        const auto new_p = Point(p.row + move.row, p.col  + move.col);
        if (!in_map(new_p, map)) continue;
        if (map[new_p.row][new_p.col] != value + 1) continue;
        if (map[new_p.row][new_p.col] == '9') {
            count++;
            continue;
        }
        search(new_p, map, count);
    }
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_10_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    int answer = 0;    
    std::vector<std::string> map;
    std::vector<Point> trailheads;
    while(std::getline(file, line)) {
        for (const auto [idx, ele] : std::views::enumerate(line)) {
            if (ele == '0') trailheads.emplace_back(map.size(), idx);
        }
        map.push_back(line);
    }

    for (const auto& trailhead : trailheads) {
        search(trailhead, map, answer);
    }
    std::cout << answer << '\n';

    return 0;

}