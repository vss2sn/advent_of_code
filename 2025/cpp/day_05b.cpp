#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Range {
    std::size_t start;
    std::size_t end;
    bool invalid = false;
    bool operator == (const Range& r) const {
        return start == r.start && end == r.end;
    }
};

bool merge(Range& r1, Range& r2) {
    if (r1.invalid || r2.invalid) return false;
    if (r1.start >= r2.start && r1.end <= r2.end) {
        r1.invalid = true;
        return true;
    }
    if (r2.start >= r1.start && r2.end <= r1.end) {
        r2.invalid = true;
        return true;
    }
    if ((r1.start >= r2.start && r1.start <= r2.end) || (r1.end >= r2.start && r1.end <= r2.end)) {
        r1.start = std::min(r1.start, r2.start);
        r1.end = std::max(r1.end, r2.end);
        r2.invalid = true;
        return true;
    }
    return false;
}

bool filter (std::vector<Range>& ranges) {
    // Do not ignore pairs of ranges if their values are the same; they should be processed unless their indices
    const auto n = ranges.size();
    std::sort(std::begin(ranges), std::end(ranges), [](const Range& r1, const Range& r2) {return !r1.invalid && r2.invalid;});
    const auto it = std::find_if(std::begin(ranges), std::end(ranges), [](const auto& r) {return r.invalid;});
    if (it != std::end(ranges)) {
        ranges.erase(it, std::end(ranges));
    }
    return (n != ranges.size());
}

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
        Range r;
        r.start = std::stoull(line.substr(0, dash_idx));
        r.end = std::stoull(line.substr(dash_idx+1, line.size() - dash_idx));
        ranges.push_back(r);   
    }
    do {
        for (std::size_t idx_1 = 0; idx_1 < ranges.size(); idx_1++) {
            auto& r1 = ranges[idx_1];
            for (std::size_t idx_2 = 0; idx_2 < ranges.size(); idx_2++) {
                if (idx_1 == idx_2) continue;
                auto& r2 = ranges[idx_2];
                merge(r1, r2);        
            }
        }
    } while(filter(ranges));
    std::size_t total_fresh_ingredients = 0;
    for (const auto& r : ranges) {
        if (!r.invalid) total_fresh_ingredients += (r.end - r.start + 1);
    }
    std::cout << total_fresh_ingredients << '\n';
}