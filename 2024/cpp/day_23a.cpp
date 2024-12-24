#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <unordered_map> 
#include <unordered_set> 
#include <vector> 

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

    std::unordered_set<std::vector<std::string>, Hasher> count;
    for (const auto& [c1, c2s] : map) {
        if (c1[0] != 't') continue;
        for (const auto& c2 : c2s) {
            for (const auto c3 : map[c2]) {
                if (c1 != c3 && map[c3].contains(c1)) {
                    std::vector<std::string> temp {c1, c2, c3};
                    std::ranges::sort(temp);
                    if(count.contains(temp)) continue;
                    count.insert(temp);

                }   
            }            
        }
    }
    std::cout << count.size() << '\n';
    return 0;
}
