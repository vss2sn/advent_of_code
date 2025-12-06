#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Range {
    std::size_t start;
    std::size_t end;
};

int main(int argc, char* argv[])
{
    std::string input = "../input/day_02_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::size_t start = 0;
    std::vector<Range> ranges;
    std::getline(file, line);
    std::size_t end_idx = 0;
    while(end_idx != line.size()) {
        const std::size_t dash_idx = line.find('-', start);
        end_idx = line.find(',', start);
        if (end_idx == std:: string::npos) end_idx = line.size();
        Range current;
        current.start = std::stoull(line.substr(start, dash_idx));
        current.end = std::stoull(line.substr(dash_idx+1, end_idx));
        ranges.push_back(current);
        start = end_idx+1;
    }
    
    std::size_t sum = 0;
    for (const auto& r : ranges) {
        for (std::size_t num = r.start; num <= r.end; num++) {
            std::size_t n = num;
            std::size_t num_size = 0;
            while (n > 0) {
                num_size++;
                n /= 10;
            }
            if (num_size % 2 != 0) continue;
            std::size_t half_num_1 = num;
            std::size_t mod_by = 1;
            for (int i = 0; i < num_size/2; i++)  {
                half_num_1 /= 10;
                mod_by *= 10;
            }
            std::size_t half_num_2 = num % mod_by;
            if (half_num_1 == half_num_2) {
                sum += num;
            }
        }
    }    
    std::cout << sum << '\n';
    return 0;
}