#include <fstream>
#include <iostream>
#include <string>
#include <ranges>

long long check(const std::string& line, const int idx) {
    static constexpr std::string numbers = "0123456789";
    const auto comma_idx = line.find_first_not_of(numbers, idx+1);
    if (comma_idx == idx+1 || comma_idx == std::string::npos || line[comma_idx] != ',') return 0;
    const auto idx_after_second_number = line.find_first_not_of(numbers, comma_idx+1);
    if (idx_after_second_number == comma_idx+1 || idx_after_second_number == std::string::npos || line[idx_after_second_number] != ')') return 0;
    return std::stoi(line.substr(idx+1, comma_idx-idx-1)) * std::stoi(line.substr(comma_idx+1, idx_after_second_number-comma_idx-1));
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_03_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::getline(file, line);

    constexpr std::string_view do_cmd_sv = "do()";
    constexpr std::string_view dont_cmd_sv = "don't()";
    constexpr std::string_view mul = "mul(";

    long long sum = 0;
    const auto line_sv = std::string_view(line);
    bool do_cmd = true;    
    for (int i = 0; i < line.size()-8; i++) {
        if (line_sv.substr(i, 4) == do_cmd_sv) do_cmd = true;
        else if (line_sv.substr(i, 7) == dont_cmd_sv) do_cmd = false;
        if (do_cmd && line_sv.substr(i, 4) == mul) {
            sum += check(line, i+3);
        }
    }
    std::cout << sum << '\n';
    return 0;
}