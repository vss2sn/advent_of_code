#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <unordered_map>
#include <vector> 

int main(int argc, char* argv[]) {
    std::string input = "../input/day_05_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    long long answer = 0;
    std::unordered_map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> updates;
    while(std::getline(file, line)) {
        if (line.empty()) break;
        const auto idx = line.find('|');
        rules[std::stoi(line.substr(0, idx))].push_back(std::stoi(line.substr(idx + 1, line.size() - idx - 1)));
    }
    while(std::getline(file, line)) {
        std::size_t current = 0;
        std::size_t comma_idx = line.find(',');
        updates.emplace_back();
        auto& update = updates.back();
        while(comma_idx != std::string::npos) {
            update.push_back(std::stoi(line.substr(current, comma_idx - current)));
            current = comma_idx + 1;
            comma_idx = line.find(',', current);
        }
        update.push_back(std::stoi(line.substr(current, line.size() - current)));
    }
    for (auto& update : updates) {
        bool follows_rules = true;
        for (int i = 0; i < update.size() && follows_rules; i++) {
            for (int j = i+1; j < update.size() && follows_rules; j++) {
                if (std::ranges::contains(rules[update[j]], update[i])) {
                    follows_rules = false;
                }
            }
        }
        if (follows_rules) {
            answer += update[update.size() / 2];
        }
    }
    
    std::cout << answer << '\n';
    return 0;
}
