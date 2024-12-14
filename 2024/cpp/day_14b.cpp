#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 
#include <limits> 

// Checked for lowest safety factor, printing out each time it reduced. 
// Factor does not seem to increase after that, at least for a while

struct Robot {
    int p_col;
    int p_row;
    int v_col;
    int v_row;
};

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
    

    std::vector<Robot> robots;
    while(std::getline(file, line)) {
        robots.emplace_back();
        std::regex_search(line, match, pattern);
        robots.back().p_col = std::stoi(match[1]) % room_col;
        robots.back().p_row = std::stoi(match[2]) % room_row;
        robots.back().v_col = std::stoi(match[3]) % room_col;
        robots.back().v_row = std::stoi(match[4]) % room_row;
    }
    long long seconds_elapsed = 1;
    int safety_factor = std::numeric_limits<int>::max();
    while(true) {
        std::array<std::array<int, room_col>, room_row> room;
        for (auto& row : room) {
            for (auto& ele : row) {
                ele = 0;
            }
        }
        std::array<int, 4> count = {{0,0,0,0}};
        for (auto& robot : robots) {
            robot.p_col += robot.v_col + room_col;
            robot.p_col %= room_col;
            robot.p_row += robot.v_row + room_row;
            robot.p_row %= room_row;
            room[robot.p_row][robot.p_col] = '#';
            if (robot.p_row < room_row / 2 && robot.p_col < room_col / 2) count[0]++;
            else if (robot.p_row < room_row / 2 && robot.p_col > room_col / 2) count[1]++;
            else if (robot.p_row > room_row / 2 && robot.p_col < room_col / 2) count[2]++;
            else if (robot.p_row > room_row / 2 && robot.p_col > room_col / 2) count[3]++;
        }
        int new_safety_factor = 1;
        for (const auto ele : count) {
            new_safety_factor *= ele;
        }
        if (new_safety_factor < safety_factor) {
            safety_factor = new_safety_factor;  
            for (const auto row : room) {
                for (const auto ele : row) {
                    std::cout << char(ele == 0 ? '.' : ele + '0');
                }
                std::cout << '\n';
            }
            std::cout << seconds_elapsed << '\n';
        }
        seconds_elapsed++;
    }
    return 0;
}