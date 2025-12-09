#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

int main(int argc, char* argv[])
{
    std::string input = "../input/day_07_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> diagram;
    while(std::getline(file, line)) {
        diagram.push_back(line);
    }
    auto cols_with_beams = std::map<int, std::size_t>{{static_cast<int>(diagram[0].find('S')), 1}};
    for (int row = 1; row < diagram.size(); row++) {
        std::map<int, std::size_t> new_cols_with_beams;
        for (const auto [idx, n_beams] : cols_with_beams) {
            if (diagram[row][idx] == '^') {
                if (idx - 1 >= 0) new_cols_with_beams[idx - 1] += n_beams;
                if (idx + 1 < diagram[0].size()) new_cols_with_beams[idx + 1] += n_beams;
            } else {
                new_cols_with_beams[idx] += n_beams;
            }
        }
        std::swap(cols_with_beams, new_cols_with_beams);
    }
    std:;size_t sum = 0;
    for (const auto& [idx, n] : cols_with_beams) {
        sum += n;
    }
    std::cout << sum << '\n';

    return 0;
}