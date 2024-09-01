#include <fstream>
#include <iostream>
#include <regex> 
#include <string>
#include <string_view>
#include <unordered_map> 
#include <unordered_set> 
#include <utility>
#include <vector>

int presents(const int n) {
    int presents = 0;
    int i = 1;
    while(i * i <= n) {
        if (n % i == 0) {
            presents += i * 10;
            const int i2 = n/i;
            if (i2 != i) presents += i2 * 10;
        }
        i++;
    }
    // std::cout << n << ": " << presents << '\n';
    return presents;
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_20_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    const int min_presents = std::stoi(line);
    int n = 0;
    while (presents (n) < min_presents) {
        n++;
    }
    std::cout << n << '\n';
    return 0;
}