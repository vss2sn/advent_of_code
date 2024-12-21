#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <string_view>
#include <unordered_set> 
#include <vector> 

// Remove primitives that can be created with smaller primitives to reduce the set of primitives
bool check_if_buildable(const std::string s, const std::unordered_set<std::string_view>& primitives, const int depth, int max_size) {
    if (s.empty()) return true;
    for (int i = max_size; i >= 1; i--) {
        if (s.size() < i) continue;
        if (primitives.contains(s.substr(0,i))) {
            if(check_if_buildable(s.substr(i, s.size() - i), primitives, depth + 1, max_size)) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_19_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> primitives;
    std::unordered_set<std::string_view> primitives_set;
    std::getline(file, line);
    std::size_t start = 0;
    std::size_t end = line.find(", ");
    std::size_t s = 0;
    while(end != std::string::npos) {
        primitives.push_back(line.substr(start, end - start));
        s = std::max(s,primitives.back().size());
        start = end + 2;
        end = line.find(", ", start);
    }
    primitives.push_back(line.substr(start, end - start));
    for (const auto& ele : primitives) {
        primitives_set.insert(ele);
    }
    std::unordered_set<std::string_view> to_remove;
    for (const auto& p : primitives) {
        if(check_if_buildable(p, primitives_set, 0, p.size() - 1)) {
            to_remove.insert(p);
        }
    }
    for (const auto& p : to_remove) {
        primitives_set.erase(p);
    }
    std::getline(file, line);
    int count = 0;
    while(std::getline(file,line)) {
        const auto buildable = check_if_buildable(line, primitives_set, 0, s);
        if (buildable) count++;
    }
    std::cout << count << '\n';;
    return 0;
}