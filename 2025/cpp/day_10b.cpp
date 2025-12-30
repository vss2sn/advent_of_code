#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <ranges>
#include <string>
#include <vector>
#include <charconv>
#include <queue>
#include <set>
#include <bitset>
#include <algorithm>
#include <unordered_map>

// While linear algebra can be used to solve this, the implementation below uses 
// 1. parity checks and 
// 2. the observation that flipping any switch twice results in reseting the lights
// Reference: https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/

template <typename T>
void print(const T& t) {
    std::cout << '(';
    for (const auto& ele : t | std::views::take(std::ranges::size(t) - 1)) {
        std::cout << ele << ", ";
    }
    std::cout << t.back();
    std::cout << ')';
}

template <typename T>
void println(const T&t) {
    print<T>(t);
    std::cout << '\n';
}

struct Machine {
    std::string lights;
    std::vector<std::vector<int>> switches;
    std::vector<int> joltage;
};

struct hasher {
    std::size_t operator() (const std::vector<int>& v) const {
        std::size_t hash = 0;
        for (int i = 0;  i < v.size(); i++) {
            hash = hash * 100 + v[i];
        }
        return hash;
    }
};

struct parity_hasher {
    std::size_t operator() (const std::vector<int>& v) const {
        std::size_t hash = 0;
        for (int i = 0; i < v.size(); i++) {
            hash = hash * 2 + (v[i] % 2);
        }
        return hash;
    }
};

std::unordered_map<std::vector<int>, 
                   std::unordered_map<std::vector<int>, int, hasher>,
                   parity_hasher> 
                   getAllParityMaps (const int n_counters, const std::vector<std::vector<int>>& switches) {
    std::unordered_map<std::vector<int>, 
                    std::unordered_map<std::vector<int>, int, hasher>,
                    parity_hasher> parity_maps;
    for (int i = 0; i < (1 << switches.size()); i++) {
        std::bitset<16> bits(i);
        auto result = std::vector<int>(n_counters, 0);
        for (int j = 0; j < switches.size(); j++) {
            if (bits[j]) {
                for (const auto ele : switches[j]) {
                    result[ele]++;
                }
            }
        }
        
        const auto parity_map = result | std::views::transform([](const auto ele) {return ele % 2; }) | std::ranges::to<std::vector<int>>();
        const int n_flips = bits.count();
        if (!parity_maps[parity_map].contains(result)) {
            parity_maps[parity_map][result] = n_flips;
        } else if (parity_maps[parity_map][result] > n_flips) {
            parity_maps[parity_map][result] = n_flips;
        }
    }
    return parity_maps;
}

std::tuple<bool, int> findMinSwitchFlips(
    const std::vector<int>& current, 
    const std::vector<std::vector<int>>& switches,
    const std::unordered_map<std::vector<int>, std::unordered_map<std::vector<int>, int, hasher>, parity_hasher>& parity_maps,
    std::unordered_map<std::vector<int>, std::tuple<bool, int>, hasher>& cache
) {
    if (cache.contains(current)) {
        return cache[current];
    }
    if (std::ranges::all_of(current, [](const auto ele) {return ele == 0;})) {
        cache[current] = {true, 0};
        return {true, 0};
    }
    if (std::ranges::any_of(current, [](const auto ele) {return ele < 0;})) {
        cache[current] = {false, 0};
        return {false, 0}; // exceeded goal
    }
    
    std::vector<int> current_parity;
    for (const auto c : current) {
        current_parity.push_back(c % 2);
    }

    if (!parity_maps.contains(current_parity)) {
        cache[current] = {false, 0};
        return {false, 0};
    }
    
    int min_flips = std::numeric_limits<int>::max();
    for (const auto& [pattern, n_flips] : parity_maps.at(current_parity)) {
        // Check validity (matching Python: all(i <= j for i, j in zip(pattern, goal)))
        bool is_valid = true;
        for (int i = 0; i < pattern.size(); i++) {
            if (pattern[i] > current[i]) {
                is_valid = false;
                break;
            }
        }
        if (!is_valid) continue;
        
        // Subtract pattern from current
        auto next = current;
        for (int i = 0; i < pattern.size(); i++) {
            next[i] = next[i] - pattern[i];
        }
        // After subtracting matching parity pattern, result is all even - divide immediately
        for (auto& ele : next) {
            ele /= 2;
        }
        
        const auto [reached_goal, count] = findMinSwitchFlips(next, switches, parity_maps, cache);
        if (reached_goal) {
            min_flips = std::min(min_flips, n_flips + 2 * count);
        }
    }
    
    if (min_flips == std::numeric_limits<int>::max()) {
        cache[current] = {false, 0};
        return {false, 0};
    }
    cache[current] = {true, min_flips};
    return {true, min_flips};
};

std::vector<int> splitAndConvertToInt(const std::string& s, const char delimiter) {
    return s
            | std::views::split(',') 
            | std::views::transform([](const auto& view) {
                int value;
                std::from_chars(view.data(), view.data() + view.size(), value);
                return value;
            })
            | std::ranges::to<std::vector<int>>();
};

std::vector<std::string> parseLine(const std::string& line) {
    std::vector<std::string> sections;
    auto start = 0;
    auto end = line.find(' ');
    while(end != std::string::npos) {
        sections.push_back(line.substr(start + (start == 0 ? 1 : 2), end - start - (start == 0 ? 2 : 3)));
        start = end;
        end = line.find(' ', start + 1);
    }
    sections.push_back(line.substr(start + 2, line.size() - start - 3));
    return sections;
}


int main(int argc, char* argv[])
{
    std::string input = "../input/day_10_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::ifstream file(input);
    std::vector<Machine> machines;
    while(std::getline(file, line)) {
        Machine m;
        const auto parsed_line = parseLine(line);
        m.lights = parsed_line[0];
        for (const auto s : parsed_line | std::views::drop(1) | std::views::take(parsed_line.size()-2)) {
            m.switches.push_back(splitAndConvertToInt(s,','));
        }
        m.joltage = splitAndConvertToInt(parsed_line.back(),',');
        machines.push_back(m);
    }

    std::size_t total = 0;
    for (const auto& m : machines) {
        const auto parity_maps = getAllParityMaps(m.joltage.size(), m.switches);
        std::unordered_map<std::vector<int>, std::tuple<bool, int>, hasher> cache;
        const auto [reached_goal, count] = findMinSwitchFlips(m.joltage, m.switches, parity_maps, cache);
        // print(m.joltage);
        // std::cout << ": " << count << '\n';
        total += count;
    }
    std::cout << total << '\n';
    return 0;
}