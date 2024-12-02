#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>

bool isSafe(const std::vector<int>& report) {
    if (report.size() == 1) {
        return true;
    }

    if(!std::ranges::is_sorted(report, std::greater{}) && !std::ranges::is_sorted(report, std::less{})) {
        return false;
    }

    for (const auto [n1, n2] : std::views::pairwise(report)) {
        const auto delta = std::abs(n1-n2);
        if(delta < 1 || delta > 3) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    std::string input = "../input/day_02_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    int safe_report_count = 0;
    while(std::getline(file, line)) {
        std::size_t current = 0;
        std::size_t space_idx = line.find(' '); 
        std::vector<int> report;
        
        // Generate report
        while(space_idx != std::string::npos) {
            report.emplace_back(std::stoi(line.substr(current, space_idx - current)));
            current = space_idx + 1; 
            space_idx = line.find(' ', current); 
        }
        report.emplace_back(std::stoi(line.substr(current, line.size() - current)));
        
        // Check for safety
        if (isSafe(report)) {
            safe_report_count++;
        }
    }
    std::cout << safe_report_count << '\n';
    return 0;
}