#include <fstream>
#include <iostream>
#include <set>
#include <string>
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
    auto cols_with_beams = std::vector<int>{static_cast<int>(diagram[0].find('S'))};
    int n_split = 0;
    for (int row = 1; row < diagram.size(); row++) {
        std::set<int> new_cols_with_beams;
        for (const auto idx : cols_with_beams) {
            if (diagram[row][idx] == '^') {
                if (idx - 1 >= 0) new_cols_with_beams.insert(idx - 1);
                if (idx + 1 < diagram[0].size()) new_cols_with_beams.insert(idx + 1);
                n_split++;
            } else {
                new_cols_with_beams.insert(idx);
            }
        }
        cols_with_beams = std::vector<int>{std::begin(new_cols_with_beams), std::end(new_cols_with_beams)};
    }
    std::cout << n_split << '\n';

    return 0;
}