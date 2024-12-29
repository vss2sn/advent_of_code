#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges> 
#include <string>
#include <set> 
#include <unordered_map> 
#include <unordered_set> 
#include <regex> 
#include <vector> 

struct Operation {
    std::string w1;
    std::string w2;
    std::string out;
    std::string op;
};

struct Compare{
    bool operator () (const Operation& op1, const Operation& op2) const {
        return op1.w1 < op2.w1 || (op1.w1 == op2.w1 && op1.w2 < op2.w2) || (op1.w1 == op2.w1 && op1.w2 == op2.w2 && op1.out < op2.out);
    }
};

bool check_wire_is_input (const std::string& w) {
    return w[0] == 'x' || w[0] == 'y';
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_24_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    const std::regex pattern(R"(([a-z0-9]+) ([A-Z]+) ([a-z0-9]+) -> ([a-z0-9]+))");
    std::smatch match;
    std::vector<Operation> operations;
    std::unordered_map<std::string, std::vector<Operation>> map;
    std::unordered_set<std::string> swapped_wires;
    int max_z = 0;
    while(std::getline(file, line)){
        std::regex_search(line, match, pattern);    
        Operation operation;
        operation.w1 = match[1];
        operation.op = match[2];
        operation.w2 = match[3];
        operation.out = match[4];
        operations.push_back(operation);
        map[operation.w1].push_back(operation);
        map[operation.w2].push_back(operation);
        if (operation.out[0] == 'z') {
            const auto idx = std::stoi(operation.out.substr(1, operation.out.size()-1));
            max_z = std::max(static_cast<int>(idx), max_z);
        }
    }
    for (auto& [wire, ops] : map) {
        std::sort(
            std::begin(ops), 
            std::end(ops), 
            [](const auto& op1, const auto& op2){
                return op1.op < op2.op;
            }
        );
    }
    std::set<std::string> incorrect_outputs;
    for (const auto& operation : operations) {
        const auto & w1 = operation.w1;
        const auto & w2 = operation.w2;
        const auto & out = operation.out;
        const auto & op = operation.op;
        if (out[0] == 'z') {
            const auto idx = std::stoi(out.substr(1, out.size() - 1));
            if (idx == 0 || idx == max_z) continue;
        }
        const bool is_input = check_wire_is_input(w1);
        const bool is_also_input = check_wire_is_input(w2);
        if (is_input && !is_also_input) {
            incorrect_outputs.insert(out);
        }
        if (op == "XOR") {
            if (is_input && out[0] == 'z') {
                incorrect_outputs.insert(out);
            }
            if (is_input && (map[out][0].op != "AND" || map[out][1].op != "XOR")) {
                incorrect_outputs.insert(out);
            }
            if (!is_input && out[0] != 'z') {
                incorrect_outputs.insert(out);
            }
        } else if (op == "AND" && (map[out].size() < 1 || map[out][0].op != "OR")) {
            incorrect_outputs.insert(out);
        } else if (op =="OR") {
            if (is_input || is_also_input) {
                incorrect_outputs.insert(out);
            }
            if (!map.contains(out) || (map[out].size() != 2 || map[out][0].op != "AND" || map[out][1].op != "XOR")) {
                incorrect_outputs.insert(out);
            }
        }
    }
    for (const auto& op : operations) {
        if (op.w1 == "x00" || op.w2 == "x00" || op.w1 == "y00" || op.w2 == "y00") {
            incorrect_outputs.erase(op.out);
        }
    }
    for (const auto & ele  : incorrect_outputs) {
        std::cout << ele << ',';
    }
    std::cout << '\n';
    return 0;
}