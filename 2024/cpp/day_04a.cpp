#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector> 

std::vector<std::pair<int, int>> moves = {
    {1,0},
    {0,1},
    {-1,0},
    {0,-1},
    {1,1},
    {1,-1},
    {-1,1},
    {-1,-1},
};

int check(const std::vector<std::string>& map, const int row, const int col) {
    int total = 0;
    for (const auto& move : moves) {
        if (col + move.second * 3 >= map[0].size()) continue;
        if (row + move.first * 3 >= map.size()) continue;
        if (col + move.second * 3 < 0) continue;
        if (row + move.first * 3 < 0) continue;
        if (map[row][col] == 'X' &&
            map[row + move.first][col + move.second] == 'M' && 
            map[row + 2*move.first][col + 2*move.second] == 'A' && 
            map[row + 3*move.first][col + 3*move.second] == 'S') {
            total += 1;
        }
    }
    return total;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_04_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> map;
    while(std::getline(file, line)) {
        map.push_back(line);
    }

    int total = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j] == 'X') {
                total += check(map, i, j);
            }
        }
    }
    std::cout << total << '\n';

    return 0;
}
