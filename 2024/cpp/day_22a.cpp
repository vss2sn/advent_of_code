#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 

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
long long generate_secret_numbers(long long secret_number, const int n) {
    // std::cout << '|' << secret_number << '|' << '\n';
    for (int i  = 0; i < n; i++) {
        secret_number = generate_next_secret_number (secret_number);
    }
    return secret_number;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_22_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    // long long test = 123;
    // for (int i = 0; i < 10; i++) {
    //     test = generate_next_secret_number(test);
    //     std::cout << test << '\n';
    // }

    long long ans = 0;
    while(std::getline(file, line)){
        long long current_ans = generate_secret_numbers(std::stoll(line), 2000);
        std::cout << current_ans << '\n';
        ans += current_ans;
    }
    std::cout << ans << '\n';
    return 0;
}
