#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <set> 
#include <map> 
#include <unordered_map> 
#include <unordered_set> 
#include <vector> 

bool check_fit(
    const std::array<std::array<char, 5>, 7>& lock, 
    const std::array<std::array<char, 5>, 7>& key) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 5; j++) {
            if (lock[i][j] == '#' && key[i][j] == '#') return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_25_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    std::vector<std::array<std::array<char, 5>, 7>> locks;
    std::vector<std::array<std::array<char, 5>, 7>> keys;


    while(std::getline(file, line)) {
        std::array<std::array<char, 5>, 7> item;
        for (const auto [idx, c] : std::views::enumerate(line)) {
            item[0][idx] = c;
        }
        for (int i = 1; i <= 6; i++) {
            std::getline(file, line);
            for (const auto [idx, c] : std::views::enumerate(line)) {
                item[i][idx] = c;
            }
        }
        // for (const auto& row : item) {
        //     for (const auto& ele : row) {
        //         std::cout << ele;
        //     }
        //     std::cout << '\n';
        // }
        // std::cout << '\n';
        if (std::all_of(item[0].begin(), item[0].end(), [](const char c) { return c == '#';})) {
            locks.push_back(item);
        } else {
            keys.push_back(item);
        }
        std::getline(file, line);
    }
    int count = 0;
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            if (check_fit(lock, key)) count++;
        }
    }
    std::cout << count << '\n';
    return 0;
}
