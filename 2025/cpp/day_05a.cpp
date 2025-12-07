#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Range {
    std::size_t start;
    std::size_t end;
};

int main(int argc, char* argv[])
{
    std::string input = "../input/day_05_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<Range> ranges;
    while(std::getline(file, line)) {
        if (line.empty()) break;
        const std::size_t dash_idx = line.find('-');
        ranges.emplace_back();
        ranges.back().start = std::stoull(line.substr(0, dash_idx));
        ranges.back().end = std::stoull(line.substr(dash_idx+1, line.size() - dash_idx));
    }
    std::unordered_set<std::size_t> n_fresh;
    while(std::getline(file, line)) {
        const std::size_t id = std::stoull(line);
        for (const auto& r : ranges) {
            if (id >= r.start && id <= r.end) {
                n_fresh.insert(id);
            }
        }
    }
    std::cout << n_fresh.size() << '\n';
}