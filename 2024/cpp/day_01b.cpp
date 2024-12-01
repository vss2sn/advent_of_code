#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
    std::string input = "../input/day_01_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<int> l1;
    std::unordered_map<int, int> l2;
    while(std::getline(file, line)) {
        std::size_t space_idx = line.find(' ');
        l1.push_back(std::stoi(line.substr(0, space_idx)));
        l2[std::stoi(line.substr(space_idx + 1, line.size() - space_idx - 1))]++;
    }
    long long sum = 0;
    for (const auto& n1 : l1) {
        sum += n1 * l2[n1];
    }
    std::cout << sum << '\n';
    return 0;
}