#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector> 

int check(const std::vector<std::string>& map, const int row, const int col) {
    int total = 0;
    static constexpr std::array<int, 2> move = {1,1};
    if (map[row][col] == 'A' &&
        ((map[row + move[0]][col + move[1]] == 'M' && map[row - move[0]][col - move[1]] == 'S') || 
            (map[row - move[0]][col - move[1]] == 'M' && map[row + move[0]][col + move[1]] == 'S')) &&
        ((map[row + move[0]][col - move[1]] == 'M' && map[row - move[0]][col + move[1]] == 'S') || 
            (map[row - move[0]][col + move[1]] == 'M' && map[row + move[0]][col - move[1]] == 'S'))) {
        total += 1;
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
    for (int i = 1; i < map.size()-1; i++) {
        for (int j = 1; j < map[0].size()-1; j++) {
            if (map[i][j] == 'A') {
                total += check(map, i, j);
            }
        }
    }
    std::cout << total << '\n';

    return 0;
}
