#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::vector<int> flatten_sequence (const std::vector<std::pair<int, int>>& sequence) {
    std::vector<int> flatten;
    for (const auto& [i,j] : sequence) {
        flatten.push_back(i);
        flatten.push_back(j);
    }
    // std::cout << "flatten: ";
    // for (const auto f : flatten) {
    //     std::cout << f << ' ';
    // }
    // std::cout << '\n';
    return flatten;
}

 std::vector<std::pair<int, int>> create_sequence(const std::vector<int>& flatten) {
    std::vector<std::pair<int, int>> sequence;
    int i = 0;
    int count = 1;
    while (i < flatten.size() - 1) {
        if (flatten[i] == flatten[i+1]) {
            count++;
        } else {
            // std::cout << "Pushing " << count << "," << flatten[i] << '\n';
            sequence.push_back({count, flatten[i]});
            count = 1;
        }
        i++;
    }
    sequence.push_back({count, flatten.back()});
    // for (const auto& [i,j] : sequence) {
    //     std::cout << i << ' ' << j << ' ';
    // }
    // std::cout << '\n';
    return sequence;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_10_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    std::vector<int> flatten;
    for (const auto c : line) {
        flatten.push_back(c - '0');
    }
    std::vector<std::pair<int, int>> sequence = create_sequence(flatten);
    for (int iter = 0; iter < (50 - 1); iter++) {
        flatten = flatten_sequence(sequence);
        sequence = create_sequence(flatten);
    }
    flatten = flatten_sequence(sequence);
    std::cout << flatten.size() << '\n';
    return 0;
}