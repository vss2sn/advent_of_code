#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector> 

// Note that the sample input will need to be updated; the '#'s need to be replaced with '.'

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
int main(int argc, char* argv[]) {
    std::string input = "../input/day_08_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    long long answer = 0;
    
    std::unordered_map<char, std::vector<Point>> antenna_type_to_locations;
    std::unordered_set<Point, Hasher> antinodes;
    std::vector<std::string> map;
    while(std::getline(file, line)) {
        map.push_back(line);
        for (const auto [idx, c] : std::views::enumerate(line)) {
            if (c != '.') {
                antenna_type_to_locations[c].emplace_back(map.size() - 1, idx);
            }
        }
    }

    for (const auto& [c, locations] : antenna_type_to_locations) {
        for (int i = 0; i < locations.size(); i++) {
            for (int j = i+1; j < locations.size(); j++) {
                const auto delta = Point(locations[i].row - locations[j].row, locations[i].col - locations[j].col);
                const auto an1 = Point(locations[i].row + delta.row, locations[i].col + delta.col);
                const auto an2 = Point(locations[j].row - delta.row, locations[j].col - delta.col);
                if (in_map(an1, map)) antinodes.emplace(locations[i].row + delta.row, locations[i].col + delta.col);
                if (in_map(an2, map)) antinodes.emplace(locations[j].row - delta.row, locations[j].col - delta.col);
            }
        }
    }
    std::cout << antinodes.size() << '\n';
    return 0;
}