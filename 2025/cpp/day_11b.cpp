#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <map>
#include <stack>
#include <unordered_set>

// Note that there are 2 sample inputs so the sample inout file will need to be adjusted

std::size_t dfs(const std::string& current, const bool visited_dac, const bool visited_fft, const std::map<std::string, std::vector<std::string>>& map) {
    static std::map<std::string, std::size_t> history;
    if (current ==  "out" && visited_dac && visited_fft) return 1;
    const auto key = current + (visited_dac ? '1' : '0') + (visited_fft ? '1' : '0');
    if (history.contains(key)) {
        return history[key];
    }
    std::size_t total = 0;
    if (!map.contains(current)) return total;
    for (const auto& to : map.at(current)) {
        total += dfs(to, visited_dac || to == "dac", visited_fft || to == "fft", map);
    }
    history[key] = total;
    return total;
}

int main(int argc, char* argv[])
{
    std::string input = "../input/day_11_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::map<std::string, std::vector<std::string>> map;
    while(std::getline(file, line)) {
        auto parsed_line = 
            line 
            | std::views::split(' ')
            | std::views::transform(std::ranges::to<std::string>());
        auto key = *(parsed_line | std::views::take(1)).begin();
        key.pop_back();
        // std::cout << key << '\n';
        map[key] = parsed_line | std::views::drop(1) | std::ranges::to<std::vector<std::string>>(); 
    }
    std::cout << dfs("svr", false, false, map) << '\n';
    return 0;
}