#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <set> 
#include <unordered_map> 
#include <unordered_set> 
#include <vector> 

// Find sets of 3, then iterate over all points and check if any is connected to all 3
// Repeat till only one set remaining

struct Hasher {
    std::size_t operator () (const std::vector<std::string>& s) const {
        return (*s.begin())[0];
    }
};

int main(int argc, char* argv[]) {
    std::string input = "../input/day_23_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    std::unordered_map<std::string, std::unordered_set<std::string>> map;
    while(std::getline(file, line)){
        std::size_t dash_idx = line.find('-');
        map[line.substr(0, dash_idx)].insert(line.substr(dash_idx + 1, line.size() - dash_idx - 1));
        map[line.substr(dash_idx + 1, line.size() - dash_idx - 1)].insert(line.substr(0, dash_idx));
    }

    std::set<std::set<std::string>> sets_of_3;
    for (const auto& [c1, c2s] : map) {
        for (const auto& c2 : c2s) {
            for (const auto c3 : map[c2]) {
                if (c1 != c3 && map[c3].contains(c1)) {
                    sets_of_3.insert({c1, c2, c3});
                }   
            }            
        }
    }

    bool new_set_size_reached = true;
    std::set<std::set<std::string>> current = sets_of_3;
    while (true) {
        std::set<std::set<std::string>> next;
        for (const auto & s : current) {
            for (const auto& [ele, connections] : map) {
                if (connections.size() < s.size()) {
                    continue;
                }
                bool contains_all = true;
                for (const auto s_ele : s) {
                    if (!map[ele].contains(s_ele)) {
                        contains_all = false;
                        break;
                    }
                }
                if (contains_all) {
                    auto superset = s;
                    superset.insert(ele);
                    next.insert(superset);
                }
            }
        }
        if (next.empty()) break;
        current = next;
    }
    std::set<std::string> lan_party_s;
    for (const auto& s : current) {
        lan_party_s = s;
    }
    std::vector<std::string> lan_party(lan_party_s.begin(), lan_party_s.end());
    std::ranges::sort(lan_party);
    for (int i = 0; i < lan_party.size() - 1; i++) {
        std::cout << lan_party[i] << ',';
    }
    std::cout << lan_party.back() << '\n';
    return 0;
}
