#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 

int main(int argc, char* argv[]) {
    std::string input = "../input/day_14_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    constexpr int time = 100;
    constexpr int room_col = 101;
    constexpr int room_row = 103;
    
    const std::regex pattern(R"(p=(-?[0-9]+),(-?[0-9]+) v=(-?[0-9]+),(-?[0-9]+))");
    std::smatch match;
    int total_cost = 0;
    std::array<int, 4> count = {{0,0,0,0}};
    std::array<std::array<int, room_col>, room_row> room;
    for (auto& row : room) {
        for (auto& ele : row) {
            ele = 0;
        }
    }
    while(std::getline(file, line)) {
        std::regex_search(line, match, pattern);
        const int p_col = std::stoi(match[1]);
        const int p_row = std::stoi(match[2]);
        const int v_col = std::stoi(match[3]);
        const int v_row = std::stoi(match[4]);
        int final_col = (p_col + v_col * time);
        while(final_col < 0) {
            final_col += (room_col * time);
        }
        final_col %= room_col;
        int final_row = p_row + v_row * time;
        while(final_row < 0) {
            final_row += (room_row * time);
        }
        final_row %= room_row;
        if (final_row < room_row / 2 && final_col < room_col / 2) count[0]++;
        else if (final_row < room_row / 2 && final_col > room_col / 2) count[1]++;
        else if (final_row > room_row / 2 && final_col < room_col / 2) count[2]++;
        else if (final_row > room_row / 2 && final_col > room_col / 2) count[3]++;
        room[final_row][final_col]++;
    }
    // for (const auto row : room) {
    //     for (const auto ele : row) {
    //         std::cout << char(ele == 0 ? '.' : ele + '0');
    //     }
    //     std::cout << '\n';
    // }
    int safety_factor = 1;
    for (const auto ele : count) {
        // std::cout << ele << ' ';
        safety_factor *= ele;
    }

    // std::cout << '\n';
    std::cout << safety_factor << '\n';
    return 0;
}