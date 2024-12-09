#include <fstream>
#include <iostream>
#include <string>
#include <vector> 

// Replace with linked list/tree/map

struct Block {
    Block(const int start_idx, const int size, const int value) : start_idx(start_idx), size(size), value(value) {}
    int start_idx;
    int size;
    int value;

    std::tuple<Block, Block> split(const int size_in, const int value_in) const {
        return {Block(start_idx, size_in, value_in), Block(start_idx + size_in, size - size_in, -1)};
    }
};

// void collapse(std::vector<Block>& blocks) {
//     for (int i = 0; i < blocks.size() - 1; i++) {
//         if (blocks[i].value == -1 && blocks[i+1].value == -1) {
//             blocks[i].size += blocks[i+1].size;
//             blocks.erase(std::begin(blocks) + i + 1);
//         }
//     }
// }

int main(int argc, char* argv[]) {
    std::string input = "../input/day_09_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    std::cout << line << '\n';
    std::vector<Block> blocks;
    int n_file = 0;
    int ptr = 0;
    int is_free = false;
    for (const auto c : line) {
        const int c_num = c - '0';
        if (is_free) {
            blocks.emplace_back(ptr, c_num, -1);
            is_free = false;
        } else {
            blocks.emplace_back(ptr, c_num, n_file);
            is_free = true;
            n_file++;
        }
        ptr += c_num ;
    }
    int idx = blocks.size() - 1;
    while (idx > 0) {
        if (blocks[idx].value == -1) {
            idx--;
            continue;
        }
        for (int idx_2 = 0; idx_2 < idx; idx_2++) {
            if (blocks[idx_2].value == -1 && blocks[idx_2].size >= blocks[idx].size) {
                const auto [b1, b2] = blocks[idx_2].split(blocks[idx].size, blocks[idx].value);
                blocks[idx_2] = b1;
                blocks[idx].value = -1;
                blocks.insert(std::begin(blocks) + idx_2 + 1, b2);
                break;
            }
        }
        idx--;
    }
    unsigned long long ans = 0;
    idx = 0;
    for (const auto& b : blocks) {
        if (b.value != - 1) {
            for (int i = 0; i < b.size; i++, idx++) {
                ans += idx * b.value;
            }
        } else {
            idx += b.size;
        }
    }

    std::cout << ans << '\n';
    return 0;
}