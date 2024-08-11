#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <map> 

int level = 0;
enum class Type {
    NONE, INT, STRING, VECTOR, MAP
};

struct Node;

struct Node {
    Type type = Type::NONE;
    int val_i;
    std::string val_s;
    std::vector<std::unique_ptr<Node>> val_v;
    std::map<std::string, std::unique_ptr<Node>> val_m;
};

std::pair<std::unique_ptr<Node>, int> parse(const int idx, const std::string& input);
std::pair<std::map<std::string, std::unique_ptr<Node>>, int> parse_map(const int idx, const std::string& input);
std::pair<std::vector<std::unique_ptr<Node>>, int> parse_vector(const int idx, const std::string& input);

std::pair<std::unique_ptr<Node>, int> parse(const int idx, const std::string& input) {
    // std::cout << "Parsing. starting at" << idx << ' ' << input[idx] << '\n';
    auto ptr = std::unique_ptr<Node>(new Node); 
    int next_idx = idx;
    if (input[idx] == '[') {
        ptr->type = Type::VECTOR;
        auto p = parse_vector(idx+1, input);
        ptr->val_v = std::move(p.first);
        next_idx = p.second;
    }
    else if (input[idx] == '{') {
        ptr->type = Type::MAP;
        auto p = parse_map(idx+1, input);
        ptr->val_m = std::move(p.first);
        next_idx = p.second;
    } 
    else if (isdigit(input[idx]) || input[idx] == '-') {
        ptr->type = Type::INT;
        next_idx = idx+1;
        while(isdigit(input[next_idx])) {
            next_idx++;
        }
        ptr->val_i = std::stoi(input.substr(idx, next_idx-idx));
    }
    else if (isalpha(input[idx]) || input[idx] == '"') {
        ptr->type = Type::STRING;
        next_idx = idx;
        while(isalpha(input[next_idx]) || input[next_idx] == '"') {
            next_idx++;
        }
        ptr->val_s = input.substr(idx, next_idx - idx);
    } 
    // if (input[next_idx] == ',') next_idx++;
    // std::cout << "Done parsing. next idx is: " << next_idx << ' ' << input[next_idx] << '\n';
    return {std::move(ptr), next_idx};
}

std::pair<std::vector<std::unique_ptr<Node>>, int> parse_vector(const int idx, const std::string& input) {
    // std::cout << "Parsing vector" << '\n';
    level++;
    std::vector<std::unique_ptr<Node>> ptrs;
    if (input[idx] == ']') {
        level--;
        return {std::move(ptrs), idx+1};
    }
    int i = idx; 
    while (i < input.size()) {
        auto ret = parse(i, input);
        ptrs.push_back(std::move(ret.first));
        if (input[ret.second] == ',') {
            i = ret.second + 1;
        } else if (input[ret.second] == ']') {
            i = ret.second + 1;
            break;
        } else {
            std::cout << "This should not happen" << '\n';
            exit(0);
        }
    }
    // std::cout << "Done parsing vector. next idx is: " << i << ' ' << input[i] << '\n';
    level--;
    return {std::move(ptrs), i};
}

std::pair<std::map<std::string, std::unique_ptr<Node>>, int> parse_map(const int idx, const std::string& input) {
    // std::cout << "Parsing map" << '\n';
    level++;
    std::map<std::string, std::unique_ptr<Node>> ptrs;
    if (input[idx] == '}') {
        level--;
        return {std::move(ptrs), idx+1};
    }
    int i = idx; 
    while (i < input.size()) {
        const int end = input.find(':', i);
        const auto key = input.substr(i, end - i);
        auto ret = parse(end + 1, input);
        ptrs[key] = std::move(ret.first);
        i = ret.second;
        if (input[ret.second] == '}') {
            i++;
            break;
        }
        else if (input[ret.second] == ',') i++;// continue the loop
        else {
            std::cout << "This should not happen" << '\n';
            exit(0);
        }
    }
    // std::cout << "Done parsing map.next idx is: " << i << ' ' << input[i] << '\n';
    level--;
    return {std::move(ptrs), i};
}

void print(std::unique_ptr<Node>& n, int spaces) {
    std::cout << '\n';
    for (int i = 0; i < spaces; i++) std::cout << "> ";
    if (n->type == Type::INT) {
        std::cout << n->val_i << '\n';
    } 
    if (n->type == Type::STRING) {
        std::cout << n->val_s << '\n';
    }
    if (n->type == Type::VECTOR) {
        for (auto & ele : n->val_v) {
            print(ele, spaces + 1);
        }
    } 
    if (n->type == Type::MAP) {
        for (auto & [key, val] : n->val_m) {
            std::cout << key << ": ";
            print(val, spaces + 1);
        }
    } 
}

void traverse(std::unique_ptr<Node>& n, int spaces, int& total) {
    if (n->type == Type::INT) {
        total += n->val_i;
    } 
    else if (n->type == Type::STRING) {
    }
    else if (n->type == Type::VECTOR) {
        for (auto & ele : n->val_v) {
            traverse(ele, spaces + 1, total);
        }
    } 
    if (n->type == Type::MAP) {
        for (auto & [key, val] : n->val_m) {
            traverse(val, spaces + 1, total);
        }
    } 
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_12_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    while(std::getline(file, line)) {
        auto ret = parse(0, line);
        if (line.size() > ret.second) {
            std::cout << "Not parsed fully" << '\n';
        }
        // auto ret = std::pair<std::unique_ptr<Node>, int>();
        // print(ret.first, 0);
        int ans = 0;
        traverse(ret.first, 0, ans);
        std::cout << ans << '\n';
    }
    
    return 0;
}