#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int count(const int row, const int col, const std::vector<std::string>& grid) {
    int total = 0;
    for (int r = row - 1; r <= row + 1; r++) {
        for(int c = col-1; c <= col+1; c++) {
            total += (grid[r][c] == '#') ? 1 : 0;
        }
    }
    total -= (grid[row][col] == '#') ? 1 : 0;
    return total;
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_18_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::vector<std::string> grid;
    while(std::getline(file, line)) {
        line = '.' + line + '.';
        grid.push_back(line);
    }
    const auto n_cols = grid[0].size();
    grid.insert(grid.begin(), std::string(n_cols, '.'));
    grid.push_back(std::string(n_cols, '.'));
    int step = 0;
    while(step < 100) {
        const auto old_grid = grid;
        for(int row = 1; row < grid.size() - 1; row++) {
            for(int col = 1; col < grid[0].size() - 1; col++) {
                const int n = count(row, col, old_grid);
                if (old_grid[row][col] == '.' && n == 3) {
                    grid[row][col] = '#';
                } else if (old_grid[row][col] == '#' && (n != 3 && n != 2)) {
                    grid[row][col] = '.';
                }
            }
        }
        step++;
    }

    int total = 0;
    for (const auto & row : grid) {
        for (const auto ele : row ) {
            total += (ele == '#') ? 1 : 0;
        }
    }
    std::cout << total << '\n';
    return 0;
}