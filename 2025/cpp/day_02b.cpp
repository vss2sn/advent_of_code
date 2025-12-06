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
        for (std::size_t number = r.start; number <= r.end; number++) {
            // Get nuber of digits
            std::size_t n_digits = 0;
            {
                std::size_t n = number;
                while (n > 0) {
                    n /= 10;
                    n_digits++;
                }
            }
            // Check for all the possible sequence sizes
            for (std::size_t seq_size = 1; seq_size < n_digits; seq_size++) {
                if (n_digits % seq_size != 0) continue;
                std::size_t order_of_sequence = 1; // order of magnitue of the sequence, 10^n_parts
                for (int i = 0; i < seq_size; i++) {
                    order_of_sequence *= 10;
                }
                const std::size_t sequence = number % order_of_sequence;
                std::size_t n = number;
                bool is_pattern = true;
                while (n > 0) {
                    // Check whether the last n_digits != sequence
                    if(n % order_of_sequence != sequence) { 
                        is_pattern = false;
                        break;
                    }
                    n /= order_of_sequence;
                }
                if (is_pattern) {
                    sum += number;
                    break;
                }
            }    
        }
    }    
    std::cout << sum << '\n';
    return 0;
}