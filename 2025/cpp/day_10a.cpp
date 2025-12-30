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
#include <unordered_set>

template <typename T>
void print(const T& t) {
    std::cout << '[';
    for (const auto& ele : t | std::views::take(std::ranges::size(t) - 1)) {
        std::cout << ele << ',';
    }
    std::cout << t.back();
    std::cout << ']';
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

struct trackSwitchFlip {
    std::string current_state;
    int s_id;
    int depth;
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

int bfs (std::queue<trackSwitchFlip>& q, const std::string& goal, const std::vector<std::vector<int>>& switches, const int depth, std::unordered_set<std::string>& seen) {
    
    while(!q.empty()) {
        auto tsf = q.front();
        q.pop();
        for (const auto i : switches[tsf.s_id]) {
            tsf.current_state[i] = (tsf.current_state[i] == '.' ? '#' : '.');
        }      
        if (tsf.current_state == goal) {
            return tsf.depth;
        }
        if (seen.contains(tsf.current_state)) continue;
        seen.insert(tsf.current_state);
        for (int i = 0; i < switches.size(); i++) {
            trackSwitchFlip new_tsf;
            new_tsf.current_state = tsf.current_state;
            new_tsf.depth = tsf.depth + 1;
            new_tsf.s_id = i;
            q.push(new_tsf);
        }
    }
    return -1;
} 

int findMinSwitchFlips(const std::string& goal, const std::vector<std::vector<int>>& switches) {
    int depth = 1;
    std::queue<trackSwitchFlip> q;
    for (int i = 0; i < switches.size(); i++) {
        trackSwitchFlip tsf;
        tsf.current_state = std::string(goal.size(), '.');
        tsf.depth = depth;
        tsf.s_id = i;
        q.push(tsf);
    }
    std::unordered_set<std::string> seen;
    return bfs(q, goal, switches, depth + 1, seen);
};

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
        total += findMinSwitchFlips(m.lights, m.switches);
    }
    std::cout << total << '\n';
    return 0;
}