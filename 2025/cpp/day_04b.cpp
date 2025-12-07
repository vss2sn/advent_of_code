#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void getAccessible(std::vector<std::vector<int>>& counter, const std::vector<std::string>& diagram) {
    // auto counter = std::vector<std::vector<int>>(diagram.size(), std::vector<int>(diagram[0].size(), 0));
    for (int row_idx = 1; row_idx < diagram.size()-1; row_idx++) {
        for (int col_idx = 1; col_idx < diagram[0].size()-1; col_idx++) {
            if (diagram[row_idx][col_idx] == '@') {
                counter[row_idx+1][col_idx]++;
                counter[row_idx][col_idx+1]++;
                counter[row_idx+1][col_idx+1]++;
                counter[row_idx-1][col_idx-1]++;
                counter[row_idx][col_idx-1]++;
                counter[row_idx-1][col_idx]++;
                counter[row_idx-1][col_idx+1]++;
                counter[row_idx+1][col_idx-1]++;
            }
        }
    }
}

std::size_t removeAccessible(const std::vector<std::vector<int>>& counter, std::vector<std::string>& diagram) {
    std::size_t count = 0;
    for (int row_idx = 1; row_idx < diagram.size()-1; row_idx++) {
        for (int col_idx = 1; col_idx < diagram[0].size()-1; col_idx++) {
            if (diagram[row_idx][col_idx] == '@' && counter[row_idx][col_idx] < 4) {
                count++;
                diagram[row_idx][col_idx] = '.';
            }
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
    std::string input = "../input/day_04_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> diagram;
    diagram.emplace_back();
    
    while(std::getline(file, line)) {
        diagram.push_back('.' + line + '.');
    }
    diagram[0] = std::string(diagram[1].size(), '.');
    diagram.push_back(diagram[0]);
    std::size_t count = 0;
    std::size_t total_count = 0;
    do {
        auto counter = std::vector<std::vector<int>>(diagram.size(), std::vector<int>(diagram[0].size(), 0));
        getAccessible(counter, diagram);
        count = removeAccessible(counter, diagram);
        total_count += count;
    } while(count > 0);
    std::cout << total_count << '\n';
    return 0;
}