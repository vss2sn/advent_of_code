#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include <limits>
#include <unordered_set>  

// Turn this chart into a pyramid expanding downwards. 
// Then each diagonal becomes a row (that starts and ends at the same index)
// The index at the end of each row is n(n+1)/2 for the row number and the index at the start is ((n-1)(n)/2 + 1) where n is the row number
// For a given row, col in the original chart, the row index in the pyramid can be found by simply adding the row and col together
// The col index in the pyramid (with the leftmost index being 0) will be the col index in the original chart
int get_idx(const int row, const int col) {
    const int row_in_pyramid = row + col; 
    return (((row_in_pyramid+1) * (row_in_pyramid))/2 + 1) + col;
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_25_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    std::regex pattern (R"(To continue, please consult the code grid in the manual.  Enter the code at row ([0-9]+), column ([0-9]+).)");
    std::smatch match;
    std::regex_search(line, match, pattern);
    const int row = std::stoi(match[1]);
    const int col = std::stoi(match[2]);
    std::unordered_set<long long> seen_s;
    std::vector<long long> seen_v;
    long long num = 20151125;
    int count = 0;
    while(!seen_s.contains(num)) {
        seen_s.insert(num);
        seen_v.push_back(num);
        num *= 252533;
        num %= 33554393;
        count++;
    }
    const int idx = get_idx(row-1, col-1); // Table row and col indices start at 1
    std::cout << seen_v[(idx - 1) % count] << '\n';
    return 0;
}