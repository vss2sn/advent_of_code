#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex> 
#include <string>
#include <unordered_map>
#include <vector>

int calculate_score(const std::vector<std::string>& current_order, const std::vector<std::string>& mapping, const std::vector<std::vector<int>>& am) {
    int total = 0;
    for (int c_i = 0; c_i < current_order.size(); c_i++) {
        const auto c_next = (c_i + 1) % current_order.size();
        const auto c_prev = (c_i - 1 + current_order.size()) % current_order.size();
        const int i = std::distance(std::begin(mapping), std::find(std::begin(mapping), std::end(mapping), current_order[c_i]));
        const int prev = std::distance(std::begin(mapping), std::find(std::begin(mapping), std::end(mapping), current_order[c_prev]));
        const int next = std::distance(std::begin(mapping), std::find(std::begin(mapping), std::end(mapping), current_order[c_next]));
        total += am[i][next] + am[i][prev] + am[next][i] + am[prev][i];
    }
    return total/2;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_13_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::regex pattern(R"(([A-Za-z]+) would (gain|lose) ([0-9]+) happiness units by sitting next to ([A-Za-z]+).)");
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> connections;
    while(std::getline(file, line)) {
        std::smatch match;
        std::regex_search(line, match, pattern);
        connections[match[1]].push_back({match[4], (match[2] == "gain" ? 1 : -1) * stoi(match[3])});
    }
    const int n_people = connections.size();
    auto am = std::vector<std::vector<int>>(n_people, std::vector<int>(n_people, 0));
    std::vector<std::string> current_order(n_people);
    for (const auto& [name_1, hps] : connections) {
        const int idx_1 = std::distance(std::begin(connections), 
                                        std::find_if(
                                            std::begin(connections), std::end(connections), 
                                                [&name_1](const auto ele) { 
                                                    return ele.first == name_1;
                                                }));
        current_order[idx_1] = name_1;
        for (const auto& [name_2, hp] : hps) {
            const int idx_2 = std::distance(std::begin(connections), 
                                        std::find_if(
                                            std::begin(connections), std::end(connections), 
                                                [&name_2](const auto ele) { 
                                                    return ele.first == name_2;
                                                }));
            am[idx_1][idx_2] = hp;
        } 
    }
    const auto mapping = current_order;
    std::sort(std::begin(current_order), std::end(current_order));
    int total = 0;
    while(std::next_permutation(std::begin(current_order), std::end(current_order))){
        total = std::max(calculate_score(current_order, mapping, am), total);
    }
    std::cout << total << '\n';
    return 0;
}   
