#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// constexpr int total_eggnog = 25;
constexpr int total_eggnog = 150;

std::vector<bool> convert_decimal_to_binary(const uint64_t n) {
    std::vector<bool> ans;
    int n_containers = 0;
    for (int i = 63; i >= 0; i--) {
        uint64_t k = n >> i;
        if (k & 1) {
            n_containers++;
            ans.push_back(true);
        } else {
            ans.push_back(false);
        }
    }
    return ans;
}

    
int calc_sum(const std::vector<int>& volumes, const std::vector<bool>& selection) {
    int total = 0;
    // for (int i = 0; i < selection.size(); i++) {
    //     std::cout << selection[selection.size() - 1 - i];
    // }
    for (int i = 0; i < volumes.size(); i++) {
        // std::cout << selection[selection.size() - 1 - i];
        if (selection[selection.size() - 1 - i]) total += volumes [volumes.size() - 1 - i];
    }
    // std::cout << '\n';
    return total;
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_17_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::vector<int> volumes;
    while(std::getline(file, line)) {
        volumes.push_back(std::stoi(line));
    }
    int count = 0;
    for (uint64_t i = 0; i < std::pow(2, volumes.size()); i++) {
        // std::cout << i << ' ';
        const auto selection = convert_decimal_to_binary(i);
        // for (const auto ele : selection) {
        //     std::cout << int(ele);
        // }
        // std::cout << ' ';
        const auto sum = calc_sum(volumes, selection);
        // std::cout << sum << '\n';
        if (sum == total_eggnog) {
            count++;
        }
    }
    std::cout << count << '\n';
    return 0;
}