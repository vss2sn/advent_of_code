#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::string input = "../input/day_03_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    int total_joltage = 0;
    while(std::getline(file, line)) {
        std::vector<int> bank;
        bank.reserve(line.size());
        for (const char c : line) {
            bank.push_back(c - '0');
        }
        const auto first_it = std::max_element(std::begin(line), std::prev(std::end(line)));
        const auto second_it = std::max_element(std::next(first_it), std::end(line));
        total_joltage += (*first_it - '0') * 10 + (*second_it - '0');
    }
    std::cout << total_joltage << '\n';
    return 0;
}