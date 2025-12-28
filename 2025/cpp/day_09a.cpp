#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::string input = "../input/day_09_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::ifstream file(input);
    std::vector<std::array<long long, 2>> points;
    while(std::getline(file, line)) {
        auto idx = line.find(',');
        const auto row = std::stoll(line.substr(0, idx));
        const auto col = std::stoll(line.substr(idx + 1, line.size() - idx - 1));
        points.push_back(std::array<long long, 2>{row, col});
    }
    std::size_t largest_rectangle = 0;
    for (const auto& p1 : points) {
        for (const auto& p2 : points) {
            largest_rectangle = std::max(
                largest_rectangle, 
                static_cast<std::size_t>(
                    (std::abs(p1[0] - p2[0])+1) * 
                    (std::abs(p1[1] - p2[1])+1)));
        }
    }    
    std::cout << largest_rectangle << '\n';
    return 0;
}