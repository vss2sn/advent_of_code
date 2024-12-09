#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <vector> 

// Replace with linked list/tree/map

int main(int argc, char* argv[]) {
    std::string input = "../input/day_09_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    std::cout << line << '\n';
    std::vector<int> blocks;
    blocks.reserve(9 * line.size());
    int n_file = 0;
    int is_free = false;
    for (const auto c : line) {
        const int c_num = c - '0';
        if (is_free) {
            for (int i = 0; i < c_num; i++) {
                blocks.push_back(-1);
            }
            is_free = false;
        } else {
            for (int i = 0; i < c_num; i++) {
                blocks.push_back(n_file);
            }
            is_free = true;
            n_file++;
        }
    }
    for (int idx = 0, r_idx = blocks.size() - 1; idx < r_idx; idx++,r_idx--) {
        while(blocks[idx] != -1) idx++;
        while(blocks[r_idx] == -1) r_idx--;
        std::swap(blocks[idx], blocks[r_idx]);
    }
    unsigned long long ans = 0;
    for (const auto [idx, val] : std::views::enumerate(blocks)) {
        if (val == -1) break;
        ans += idx * (val);
    }

    std::cout << ans << '\n';
    return 0;
}