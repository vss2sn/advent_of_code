#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char * argv[]) {
    std::string input = "../input/day_02_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    const std::regex pattern(R"(([0-9]+)x([0-9]+)x([0-9]+))");
    int total = 0;
    while(std::getline(file, line)) {
        std::smatch match;
        std::regex_search(line, match, pattern);
        // Sample input
        const auto area_1 = (std::stoi(match[1]) * std::stoi(match[2]));
        const auto area_2 = (std::stoi(match[2]) * std::stoi(match[3]));
        const auto area_3 = (std::stoi(match[3]) * std::stoi(match[1]));
        const auto slack = std::min(std::min(area_1, area_2), area_3);
        total += (area_1 + area_2 + area_3) * 2 + slack;
    }
    std::cout << total << '\n';
    return 0;
}