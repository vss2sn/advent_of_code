#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 

std::tuple<bool, long long, long long> check_linear_combination(const long long ax, const long long ay, const long long bx, const long long by, const long long cx, const long long cy) {
    if ((ax * by) == (bx * ay)) exit(0);
    const long long n1 = (cx * by - cy * bx) / (ax * by - ay * bx);
    const long long n2 = (cx * ay - cy * ax) / (bx * ay - by * ax);
    // Account for integer rounding
    if (n1 * ax + n2 * bx != cx) return {false, -1, -1};
    if (n1 * ay + n2 * by != cy) return {false, -1, -1};
    if (n1 < 0 || n2 < 0) return {false, -1, -1};
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
    long long total_cost = 0;
    while(true) {
        std::getline(file, line);
        std::regex_search(line, match, pattern);
        const long long ax = std::stoi(match[1]);
        const long long ay = std::stoi(match[2]);
        std::getline(file, line);
        std::regex_search(line, match, pattern);
        const long long bx = std::stoi(match[1]);
        const long long by = std::stoi(match[2]);
        std::getline(file, line);
        std::regex_search(line, match, pattern);
        const long long px = std::stoi(match[1]) + 10000000000000;
        const long long py = std::stoi(match[2]) + 10000000000000;
        const auto [possible, n1, n2] = check_linear_combination(ax, ay, bx, by, px, py);
        // std::cout << (possible ? "yes" : "no") << ' ' << n1  << ' ' << n2 << ' ' << n1 * 3 + n2 * 1 << '\n';
        if (possible) total_cost += (n1 * 3 + n2);
        if(!std::getline(file, line)) break;
    }
    std::cout << total_cost << '\n';
    return 0;
}
