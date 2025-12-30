#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <map>
#include <stack>
#include <unordered_set>

// Note that there are 2 sample inputs so the sample inout file will need to be adjusted

std::size_t dfs(const std::string& current, const std::map<std::string, std::vector<std::string>>& map) {
    if (current ==  "out") return 1;
    const auto key = current;
    std::size_t total = 0;
    if (!map.contains(current)) return total;
    for (const auto& to : map.at(current)) {
        total += dfs(to, map);
    }
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
        map[key] = parsed_line | std::views::drop(1) | std::ranges::to<std::vector<std::string>>(); 
    }
    std::cout << dfs("you", map) << '\n';
    return 0;
}