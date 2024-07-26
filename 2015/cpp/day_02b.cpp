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
        const auto perimeter_1 = (std::stoi(match[1]) + std::stoi(match[2])) * 2;
        const auto perimeter_2 = (std::stoi(match[2]) + std::stoi(match[3])) * 2;
        const auto perimeter_3 = (std::stoi(match[3]) + std::stoi(match[1])) * 2;
        const auto smallest_perimeter = std::min(std::min(perimeter_1, perimeter_2), perimeter_3);
        total += smallest_perimeter + (std::stoi(match[1]) * std::stoi(match[2]) * std::stoi(match[3]));
    }
    std::cout << total << '\n';
    return 0;
}