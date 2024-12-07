#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <vector> 

template<long long base>
bool incrementNumberWithBase(std::vector<long long>& num) {
    long long carry = 1;
    for (auto& ele : std::views::reverse(num)) {
        if (carry == 1) {
            ele += 1;
            if (ele == base) {
                carry = 1;
                ele = 0;
            } else {
                carry = 0;
            }
        }
    }
    return (carry != 1); 
    // If this is 1 then exceeded the length of the vector 
    // If the vector passed in has a size == the number of operators between digits, then no longer need to increment
}

bool solve(const std::vector<long long>& equation) {
    if (equation.size() == 2) {
        return equation[0] == equation[1];
    }
    auto operators = std::vector<long long>(equation.size() - 2, 0);
    do {
        long long ans = equation[1];
        for (const auto [idx, ele] : std::views::enumerate(operators)) {
            if (ele == 0) {
                ans += equation[idx + 2];
            } else if (ele == 1) {
                ans *= equation[idx + 2];
            }
        }
        if (ans == equation[0]) return true;
    } while (incrementNumberWithBase<2>(operators));
    return false;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_07_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::vector<long long>> equations;
    long long ans = 0;
    static constexpr std::string digits = "0123456789";
    while(std::getline(file, line)) {
        equations.emplace_back();
        auto& equation = equations.back();
        std::size_t current = 0;
        std::size_t end_idx = line.find_first_not_of(digits, current);
        while(end_idx != std::string::npos) {
            equation.push_back(std::stoll(line.substr(current, end_idx - current)));
            current = line.find_first_of(digits, end_idx);
            end_idx = line.find_first_not_of(digits, current);
        }
        equation.push_back(std::stoll(line.substr(current, line.size() - current)));
    }
    for (const auto equation : equations) {
        if(solve(equation)) {
            ans += equation[0];
        } 
    }
    std::cout << ans << '\n';
    return 0;
}