#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 

std::tuple<bool, int, int> check_linear_combination(const int ax, const int ay, const int bx, const int by, const int cx, const int cy) {
    if ((ax * by) == (bx * ay)) exit(0);
    const int n1 = (cx * by - cy * bx) / (ax * by - ay * bx);
    const int n2 = (cx * ay - cy * ax) / (bx * ay - by * ax);
    // Account for integer rounding
    if (n1 * ax + n2 * bx != cx) return {false, -1, -1};
    if (n1 * ay + n2 * by != cy) return {false, -1, -1};
    if (n1 < 0 || n2 < 0 || n1 > 100 || n2 > 100) return {false, -1, -1};
    return {true, n1, n2};
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_13_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    long long answer = 0;
    
    const std::regex pattern(R"([A-Za-z ]+: X[+=]([0-9]+), Y[+=]([0-9]+))");
    std::smatch match;
    int total_cost = 0;
    while(true) {
        std::getline(file, line);
        std::regex_search(line, match, pattern);
        const int ax = std::stoi(match[1]);
        const int ay = std::stoi(match[2]);
        std::getline(file, line);
        std::regex_search(line, match, pattern);
        const int bx = std::stoi(match[1]);
        const int by = std::stoi(match[2]);
        std::getline(file, line);
        std::regex_search(line, match, pattern);
        const int px = std::stoi(match[1]);
        const int py = std::stoi(match[2]);
        const auto [possible, n1, n2] = check_linear_combination(ax, ay, bx, by, px, py);
        // std::cout << (possible ? "yes" : "no") << ' ' << n1  << ' ' << n2 << ' ' << n1 * 3 + n2 * 1 << '\n';
        if (possible) total_cost += (n1 * 3 + n2);
        if(!std::getline(file, line)) break;
    }
    std::cout << total_cost << '\n';
    return 0;
}
