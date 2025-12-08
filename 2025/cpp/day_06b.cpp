#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::string input = "../input/day_06_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    
    std::vector<std::string> parsed_input;
    std::size_t longest_size = 0;
    
    // Parse input
    while(std::getline(file, line)) {  
        if (line[0] == '+' || line[0] == '-' || line[0] == '*' || line[0] == '/') break;
        for (auto& c : line) {
            if (c == ' ') c = '.';
        }
        longest_size = std::max(longest_size, line.size());
        parsed_input.push_back(line);
    }

    const std::string signs = line;
    // make sure length of the lines in the input is the same
    for (auto& line : parsed_input) {
        while (line.size() < longest_size) {
            line += '.';
        }
    }

    // find column spaces of the next set of numbers
    std::size_t start = 0;
    std::size_t end = end = signs.find_first_not_of(' ', start + 1);
    std::vector<std::size_t> start_indices;
    do {
        const auto sign = signs[start];
        // std::cout << sign << '\n';
        end = signs.find_first_not_of(' ', start + 1);
        start_indices.push_back(start);
        start = end;
    } while (end != std::string::npos);
    start_indices.push_back(longest_size+1);
    // for (const auto& ele : start_indices) {
    //     std::cout << ele << ' ';
    // }
    // std::cout << '\n';
    std::vector<std::string> numbers_str(longest_size);
    for (const auto& row : parsed_input) {
        for (int i = 0; i < longest_size; i++) {
            // std::cout << i << ": " << numbers_str[i] << '\n';
            numbers_str[i] += row[i];
        }
    }
    // for  (const auto& n : numbers_str) {
    //     std::cout << n << '\n';
    // }
    const auto remove_char = [](std::string& s) {
        s.erase(std::remove(s.begin(), s.end(), '.'), s.end());
        return s;
    };
    
    std::size_t total = 0;
    for (int i = 0; i < start_indices.size()-1; i++) {
        const auto start = start_indices[i];
        const auto end = start_indices[i+1]-1;
        // std::cout << "start" << start << '\n';
        // std::cout << "end" << end << '\n';
        const auto sign = signs[start];
        // std::cout << "sign: " << sign << '\n';
        std::size_t ans = 0;
        if (sign == '+') {
            ans = std::accumulate(
                std::next(std::begin(numbers_str), start),
                std::next(std::begin(numbers_str), end),
                std::size_t(0),
                [&](std::size_t sum, std::string ele) { 
                    // std::cout << "sum: " << sum << '\n';
                    // std::cout << "ele: " << remove_char(ele) << '\n';
                    return std::stoull(remove_char(ele)) + sum; 
                }
            );
        } 
        else if (sign == '*') {
            ans = std::accumulate(
                std::next(std::begin(numbers_str), start),
                std::next(std::begin(numbers_str), end),
                std::size_t(1),
                [&](std::size_t prod, std::string ele) { 
                    // std::cout << "prod: " << prod << '\n';
                    // std::cout << "ele: " << remove_char(ele) << '\n';
                    return std::stoull(remove_char(ele)) * prod; 
                }
            );
        }
        // std::cout << ans << '\n';
        total += ans;
    }
    std::cout << total << '\n';
    
}