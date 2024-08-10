#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// useful for debug
enum class InvalidReason {
    NONE,
    NO_THREE_INCREASING,
    CONTAINS_FORBIDDEN,
    NO_PAIRS
};

std::tuple<bool, InvalidReason, int> is_valid(const std::string& password) {
    bool three_increasing = false;
    for (int i = 0; i < password.size()-2; i++) {
        if (password[i] + 1 == password[i+1] && password[i]+2 == password[i+2]) {
            three_increasing = true;
            break;
        }
    }
    if (!three_increasing) return {false, InvalidReason::NO_THREE_INCREASING, -1};
    for (int i = 0; i < password.size(); i++) {
        const auto c = password[i];
        if (c == 'i' || c == 'o' || c == 'l') return {false, InvalidReason::CONTAINS_FORBIDDEN, i};
    }
    std::vector<std::pair<char, int>> pairs;
    for(int i = 0; i < password.size()-1; i++) {
        if (password[i] == password[i+1]) {
            pairs.push_back({password[i], i});
            for (int j = pairs.size() - 2; j >= 0; j--) {
                if (pairs.back().first != pairs[j].first) return {true, InvalidReason::NONE, i};
            }
        }
    }
    return {false, InvalidReason::NO_PAIRS, -1};
}

void increment (std::string& password) {
    for (int i = password.size() - 1; i >= 0; i--) {
        password[i] += 1;
        if (password[i] > 'z') {
            password[i] = 'a';
        } else if (password[i] == 'i' || password[i] == 'o' || password[i] == 'l') {
            password[i] += 1;
            for (int j = i + 1; j < password.size(); j++) {
                password[j] = 'a';
            } 
            return;
        }
        else {
            return;
        }
    }
}

void get_next_password(std::string& password) {
    increment(password);
    auto ret = is_valid(password);
    while(!std::get<0>(ret)) {
        // std::cout << password << " -> " << static_cast<int>(std::get<1>(ret)) << '\n';
        increment(password);
        ret = is_valid(password);
    }
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_11_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string password;
    while(std::getline(file, password)) {
        // To handle sample input
        // std::cout << password << ": " << std::boolalpha << std::get<0>(is_valid(password)) << std::noboolalpha << '\n';
        const  auto [valid, reason, idx] = is_valid(password);
        // if (!valid) continue;
        get_next_password(password);
        get_next_password(password);
        std::cout << password << '\n';
    }
    return 0;
}