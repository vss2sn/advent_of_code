#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// do
// b = a % 8;
// b = b bitwise xor 3
// c = a / (2^b)
// b = b bitwise xor 5
// a = a / 2^3
// b = b bitwise xor c
// std::cout << b % 8
// while (a!=0)

// Key observation: each 3 bits are responsible for 1 number in the puzzle input
// Solution outline: 
// use dfs to find a number that (on passing it through the reverse engineering code) will produce the last digit of the program
// left shift by 3 bits
// check whether adding any of 0 to 7 and passing it through the reverse engineering code produces the last 2 digits of the program
// left shift by 3 bits
// check whether adding any of 0 to 7 and passing it through the reverse engineering code produces the last 3 digits of the program
// and so on
// 0 to 7 since those are the only numbers that bit in 3 bits

std::vector<int> f(const long long n) {
    std::vector<int> output;
    long long a = n;
    long long b = 0;
    long long c = 0;
    do{
        b = a % 8;
        b = b ^ 3;
        c = a / static_cast<long long>(std::pow(2,b));
        b = b ^ 5;
        a = a / 8;
        b = b ^ c;
        for (const auto ele : std::to_string(b % 8)) {
            output.push_back(ele - '0');
        }
    } while (a!=0);
    return output;
}

bool util (long long n, const std::vector<int>& program) {
    if (f(n) == program) {
        std::cout << n << '\n';
        return true;
    }
    n = n << 3;
    for (int i = 0; i < 8; i++) {
        const auto output = f(n);
        bool same = true;
        for (int i = 0; i < output.size(); i++) {
            same = same && output[i] == program[program.size() - output.size() + i];
        }
        if (same && util(n, program)) return true;
        n++;
    }
    return false;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_17_input";
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;

    for (int i = 0; i < 5; i++) std::getline(file, line);
    std::vector<int> program;
    std::size_t start = 9;
    std::size_t end = line.find(',', start);
    while(end != std::string::npos) {
        program.push_back(std::stoi(line.substr(start, end - start)));
        start = end + 1;
        end = line.find(',', start);
    }
    program.push_back(std::stoi(line.substr(start, line.size() - start)));
    util(0, program);
}
