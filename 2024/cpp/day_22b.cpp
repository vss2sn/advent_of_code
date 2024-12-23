#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 
#include <set>
#include <map>
#include <numeric>


void mix (long long& secret_number, long long n) {
    secret_number = secret_number ^ n;
}

void prune(long long& secret_number) {
    secret_number = secret_number % 16777216;
}

long long generate_next_secret_number (long long secret_number) {
    // Op1 
    mix(secret_number,  secret_number * 64);
    prune(secret_number);
    // Op2
    mix(secret_number, secret_number/32);
    prune(secret_number);
    // op3
    mix(secret_number, secret_number * 2048);
    prune(secret_number);
    return secret_number;
}

template<int N> 
std::map<std::array<long long, 4>, long long> generate_secret_numbers(long long secret_number) {
    std::map<std::array<long long, 4>, long long> sequences;
    std::array<long long, N+1> secret_numbers;
    std::array<long long, N+1> diffs;
    for (long long i  = 0; i < N; i++) {
        secret_numbers[i] = secret_number;
        secret_number = generate_next_secret_number (secret_number);
    }
    secret_numbers[N] = secret_number;
    for (auto & n : secret_numbers) {
        n %= 10;
    }
    std::adjacent_difference(std::begin(secret_numbers), std::end(secret_numbers), std:: begin(diffs));
    for (long long i = 1; i < diffs.size() - 4; i++) {
        // std::cout << "----" << '\n';
        // std::cout << secret_numbers[i-1] << ' ' << secret_numbers[i] << ' ' <<  secret_numbers[i+1] << ' ' <<  secret_numbers[i+2] << ' ' <<  secret_numbers[i+3]  << ' ' << '\n';;
        // std::cout << diffs[i] << ' ' <<  diffs[i+1] << ' ' <<  diffs[i+2] << ' ' <<  diffs[i+3] << ' ' << '\n';
        // std::cout << '\n';
        std::array<long long, 4> sequence{{diffs[i], diffs[i+1], diffs[i+2], diffs[i+3]}};
        if (sequences.contains(sequence)) continue;
        sequences[sequence] = secret_numbers[i+3];
    }
    return sequences;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_22_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    // long long test = 123;
    // auto temp = generate_secret_numbers<10>(test);
    // for(const auto & [n1, n2] : temp) {
    //     for (const auto ele : n1) {
    //         std::cout << ele << ' '; 
    //     }
    //     std::cout << ": " << n2 << '\n';
    // }
    // exit(0);


    std::map<std::array<long long, 4>, std::vector<long long>> sequences_for_all_buyers;
    while(std::getline(file, line)){
        const auto sequences_for_buyer = generate_secret_numbers<2000>(std::stoll(line));
        for (const auto& [sequence, bananas] : sequences_for_buyer) {
            // for (const auto& ele : sequence) {
            //     std::cout << ele << ' ';
            // }
            // std::cout << ": " << bananas << '\n';
            sequences_for_all_buyers[sequence].push_back(bananas);
        }
    }
    std::array<long long, 4> best_sequence = {{0,0,0,0}};
    long long best_total = 0;
    for (const auto& [sequence, bananas_from_all_buyers] : sequences_for_all_buyers) {
        long long total = 0;
        for (const auto bananas_from_each_buyer : bananas_from_all_buyers) {
            total += bananas_from_each_buyer;
        }
        // for (const auto& ele : sequence) {
        //     std::cout << ele << ' ';
        // }
        // std::cout << ": " << total << '\n';
        if (best_total < total) {
            best_total = total;
            best_sequence = sequence;
        }
    }   
    for (const auto& ele : best_sequence) {
        std::cout << ele << ' ';
    }
    std::cout << ": " << best_total << '\n';
    return 0;
}
