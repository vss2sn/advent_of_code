#include <fstream>
#include <iostream>
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
    
    std::vector<std::vector<int>> numbers;
    std::vector<char> signs;
    while(std::getline(file, line)) {  
        if (line[0] == '+' || line[0] == '-' || line[0] == '*' || line[0] == '/') break;
        numbers.emplace_back();
        std::size_t start = line.find_first_not_of(' ');
        std::size_t end = 0;
        while(start != std::string::npos && end != std::string::npos) {
            end = line.find(' ', start);
            numbers.back().emplace_back( std::stoi(line.substr(start, end)));   
            start = line.find_first_not_of(' ', end + 1);
        }  
    }
    // for (const auto& row : numbers) {
    //     for (const auto& ele : row) {
    //         std::cout << ele << ' ';
    //     }
    //     std::cout << '\n';
    // }
    std::size_t start = line.find_first_not_of(' ');
    while(start != std::string::npos) {
        const std::size_t end = line.find_first_not_of(' ', start);
        signs.emplace_back(line[end]);   
        start = line.find_first_not_of(' ', end + 1);
    }
    // for (const auto& ele : signs) {
    //     std::cout << ele << ' ';
    // }
    // std::cout << '\n';
    
    std::size_t sum = 0;
    for (std::size_t idx = 0; idx < signs.size(); idx++) {
        std::size_t val = numbers[0][idx];
        for (std::size_t idx_2 = 1; idx_2 < numbers.size(); idx_2++) {
            if (signs[idx] == '+') val += numbers[idx_2][idx];
            else if (signs[idx] == '-') val -= numbers[idx_2][idx];
            else if (signs[idx] == '*') val *= numbers[idx_2][idx];
            else if (signs[idx] == '/') val /= numbers[idx_2][idx];
        }
        // std::cout << val << '\n';
        sum += val;
    }
    std::cout << sum << '\n';
}