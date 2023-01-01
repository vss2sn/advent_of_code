#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue> 
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

// TODO: Clean up and optimize
//       Also, use a better method

std::vector<long long> powers_5{1};

long long convert_snafu_to_b10(const std::string& n) {
    long long ans = 0;
    while (n.size() > powers_5.size()) {
        powers_5.push_back(5 * powers_5.back());
    }
    for (int i = n.size()-1; i >= 0; i--) {
        const auto multiplier = powers_5[n.size() - i - 1];
        if (n[i] == '=') {
            ans += -2 * multiplier;
        } 
        else if (n[i] == '-') {
            ans += -1 * multiplier;
        }
        else if (n[i] == '0') {
            ans += 0;
        }
        else if (n[i] == '1') {
            ans += 1 * multiplier;
        }
        else if (n[i] == '2') {
            ans += 2 * multiplier;
        } else {
            std::cout << "This should not happen" << '\n';
            exit(0);
        }
    }
    return ans;
}

// long long too small to contain rep in numbers
std::string convert_b10_to_b5(long long n) {
    std::string ans;
    while (n > 0) {
        const auto rem = n % 5;
        ans.push_back(rem + '0');
        n = n / 5;
    }
    std::reverse(std::begin(ans), std::end(ans));
    return ans;
}

std::string convert_b5_to_snafu(std::string n) {
    std::string ans;
    for (int i = n.size() -1; i >= 0; i-- ) {
        // std::cout <<  n.substr(0, i+1) << '\n';
        const auto rem = (n[i] - '0') % 5;
        if (rem == 0 || rem == 1 || rem == 2) {
            ans.push_back(rem + '0');
        } else if (rem == 3) {
            ans.push_back('=');
            if (i != 0) {
                n[i-1] += 1;
            } else {
                n = "1";
            }
        } else {
            ans.push_back('-');
            if (i != 0) {
                n[i-1] += 1;
            } else {
                n = "1";
            }
        }
        auto j = i - 1;
        while(j >=0 && n[j] == '5') {
            // std::cout <<  "Carried, so becomes: << n << '\n';
            n[j] = '0';
            if (j >= 1) {
                n[j-1] += 1;
            } else {
                n = "1" + n;
                n[1] = '0';
            }
            j--;
        }
        // std::cout << "End: " << n.substr(0, i+1) << '\n';
    }
    std::reverse(std::begin(ans), std::end(ans));
    return ans;
}


int main(int argc, char * argv[]) {
    std::string input = "../input/day_25_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::fstream file(input);

    std::vector<std::string> numbers;

    while(std::getline(file, line)) {
        numbers.push_back(line);
    }
    long long total = 0;
    for (const auto& snafu : numbers) {
        total += convert_snafu_to_b10(snafu);
    }
    // std::cout << total << '\n';
    std::cout << convert_b5_to_snafu(convert_b10_to_b5(total)) << '\n';
    return 0;
}