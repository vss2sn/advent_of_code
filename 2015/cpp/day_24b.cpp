#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>  
#include <vector>

// find all possible combinations of presents that can form a group
void iterate(const std::vector<int>& weights, int current_weight, std::vector<int>& combination, int idx, const int avg, std::vector<std::vector<int>>& combinations) {
    for (int i = idx; i < weights.size(); i++) {
        if (current_weight + weights[i] < avg) {
            combination.push_back(weights[i]);
            iterate(weights, current_weight + weights[i], combination, i + 1, avg, combinations);
            combination.pop_back();
        } else if (current_weight + weights[i] == avg) {
            combination.push_back(weights[i]);
            combinations.push_back(combination);
            combination.pop_back();
        } 
    }
}

bool overlap(const std::vector<int>& v1, const std::vector<int>& v2) {
    for (const auto ele_v1 : v1) {
        if (std::find(v2.begin(), v2.end(), ele_v1) != v2.end()) {
            return true;
        }
    }
    return false;
}

uint64_t calc_qa(const std::vector<int>& combination) {
    uint64_t t = 1;
    for (const auto ele : combination) {
        t *= ele;
    }
    return t;
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_24_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::vector<int> weights;
    int avg = 0;
    while(std::getline(file, line)) {
        const auto weight = std::stoi(line);
        avg += weight;
        weights.push_back(weight);
    }
    avg /= 4;
    // find all possible combinations of presents that can form a group
    std::vector<std::vector<int>> combinations;
    {
        std::vector<int> combination;
        iterate(weights, 0, combination, 0, avg, combinations);
    }
    // Sort the possible combinations smallest to largest, with ties broken by lowest quantum entanglement
    std::sort(std::begin(combinations), std::end(combinations), [](const auto& c1, const auto& c2) {
        if(c1.size() < c2.size()) return true;
        if(c1.size() > c2.size()) return false;
        return calc_qa(c1) < calc_qa(c2); // Primes ensure product is always unique
    });
    // Find the first combination of weights that does not overlap with at least one other combination of weights
    // If there exist 2 combinations of presents that each equal the average 
    // then it stands to reason that the remaining presents will form a third group 
    // which will also equal the average
    for (int i = 0; i < combinations.size(); i++) {
        for (int j = i+1; j < combinations.size(); j++) {
            if (!overlap(combinations[i], combinations[j])) {
                std::cout << calc_qa(combinations[i]) << '\n';
                return 0;
            }
        }   
    }
    std::cout << "No solution found" << '\n';
    return 0;
}