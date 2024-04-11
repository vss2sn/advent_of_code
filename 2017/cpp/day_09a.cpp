#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string input = (argc > 1) ? argv[1] : "input";
    std::ifstream file(input);
    std::string line;
    int total_score = 0; // 总分数

    while (std::getline(file, line)) {
        int idx = 0;
        bool is_in_garbage = false;
        int level = 0;
        int score = 0;
        int groups = 0;

        while (idx < line.size()) {
            if (!is_in_garbage) {
                if (line[idx] == '{') {
                    level++;
                    score += level;
                    groups++;
                } else if (line[idx] == '}') {
                    level--;
                } else if (line[idx] == '<') {
                    is_in_garbage = true;
                }
            } else {
                if (line[idx] == '!') {
                    idx++; // 跳过下一个字符
                } else if (line[idx] == '>') {
                    is_in_garbage = false;
                }
            }
            idx++;
        }
        total_score += score; // 累加当前行的分数
    }

    std::cout << "Total score: " << total_score << '\n';
    return 0;
}
