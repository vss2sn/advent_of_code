#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string input = (argc > 1) ? argv[1] : "input";
    std::ifstream file(input);
    std::string line;
    int total_garbage_chars = 0; // 垃圾中的非取消字符数量

    while (std::getline(file, line)) {
        int idx = 0;
        bool is_in_Garbage = false;
        int garbage_chars = 0; // 当前垃圾中的字符数量

        while (idx < line.size()) {
            if (!is_in_Garbage) {
                if (line[idx] == '<') {
                    is_in_Garbage = true;
                }
            } else {
                if (line[idx] == '!') {
                    idx++; // 跳过下一个字符
                } else if (line[idx] == '>') {
                    is_in_Garbage = false;
                    total_garbage_chars += garbage_chars; // 累加当前垃圾中的字符数量
                    garbage_chars = 0; // 重置当前垃圾中的字符数量
                } else {
                    garbage_chars++; // 计算垃圾中的字符数量
                }
            }
            idx++;
        }
    }

    std::cout << "Total non-canceled characters in garbage: " << total_garbage_chars << '\n';
    return 0;
}
