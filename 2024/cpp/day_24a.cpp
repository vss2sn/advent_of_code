#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges> 
#include <string>
#include <set> 
#include <map> 
#include <unordered_map> 
#include <unordered_set> 
#include <regex> 
#include <vector> 
#include <bitset> 

enum class Operator {
    AND, OR, XOR
};

struct Operation {
    std::string w1;
    std::string w2;
    std::string out;
    Operator op;
};

int main(int argc, char* argv[]) {
    std::string input = "../input/day_24_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    std::unordered_map<std::string, bool> wire_vals;
    while(std::getline(file, line)){
        if (line.empty()) break;
        std::size_t colon_idx = line.find(": ");
        wire_vals[line.substr(0, colon_idx)] = ((line[colon_idx + 2] - '0') == 1);
    }
    const std::regex pattern(R"(([a-z0-9]+) ([A-Z]+) ([a-z0-9]+) -> ([a-z0-9]+))");
    std::smatch match;
    std::vector<Operation> operations;
    while(std::getline(file, line)){
        std::regex_search(line, match, pattern);    
        Operation operation;
        operation.w1 = match[1];
        operation.w2 = match[3];
        operation.out = match[4];
        if (match[2] == "AND") operation.op = Operator::AND;
        else if (match[2] == "OR") operation.op = Operator::OR;
        else if (match[2] == "XOR") operation.op = Operator::XOR;
        operations.push_back(operation);
    }

    bool changed = true;
    while(changed) {
        changed = false;
        for (const auto& operation : operations) {
            std::cout << operation.w1 << ' ' << operation.w2 << ' ' << operation.out << '\n';
            if (wire_vals.contains(operation.out)) {
                continue;
            } else if (wire_vals.contains(operation.w1) && 
                    wire_vals.contains(operation.w2)) {
                if (operation.op == Operator::AND) {
                    wire_vals[operation.out] = (wire_vals[operation.w1] && wire_vals[operation.w2]);
                    std::cout << wire_vals[operation.out] << '\n';
                    changed = true;
                }
                else if (operation.op == Operator::OR) {
                    wire_vals[operation.out] = (wire_vals[operation.w1] || wire_vals[operation.w2]);
                    changed = true;
                }
                else if (operation.op == Operator::XOR) {
                    wire_vals[operation.out] = ((!wire_vals[operation.w1]) != (!wire_vals[operation.w2]));
                    changed = true;
                }
            }
        }
    }

    std::bitset<32> z_values;
    std::vector<std::pair<std::string, int>> temp;
    for (const auto& [w, val] : wire_vals) {
        // std::cout << w << ": " << val << '\n';
        temp.push_back({w, val});
        if (w[0] == 'z') {
            z_values[std::stoi(w.substr(1, w.size() - 1))] = val;
        }
    }
    std::ranges::sort(temp);
    for (const auto& [w, val] : temp) {
        std::cout << w << ": " << val << '\n';
    }
    std::cout << z_values.to_string() << '\n';
    std::cout << z_values.to_ulong() << '\n';
    return 0;
}