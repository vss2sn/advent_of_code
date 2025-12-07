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
    std::size_t total_joltage = 0;
    while(std::getline(file, line)) {
        std::vector<std::size_t> bank;
        bank.reserve(line.size());
        for (const char c : line) {
            bank.push_back(c - '0');
        }
        std::size_t joltage = 0;
        auto it = std::begin(line);
        const int n_digits = 12;
        for (int i = 0; i < n_digits; i++) {
            it = std::max_element(it, std::prev(std::end(line), n_digits - i - 1));
            joltage = joltage * 10 + (*it - '0');
            it = std::next(it);
        }
        total_joltage += joltage;
    }
    std::cout << total_joltage << '\n';
    return 0;
}