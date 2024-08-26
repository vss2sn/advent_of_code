#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional> 
#include <string>

struct Sue {
    int id;
    std::optional<int> children;
    std::optional<int> cats;
    std::unordered_map<std::string, int> dogs;
    std::optional<int> goldfish;
    std::optional<int> trees;
    std::optional<int> cars;
    std::optional<int> perfumes;
};

Sue parse(std::string& line) {
    line.erase(std::remove_if(line.begin(), line.end(), [](const char c) {return c == ' ';}), line.end());
    Sue aunt;
    auto idx = line.find(':');
    aunt.id = std::stoi(line.substr(3, idx - 3));
    std::string item;
    int val = 0;
    while (idx != std::string::npos) {
        auto next_idx = line.find(':', idx+1);
        item = line.substr(idx+1, next_idx - idx-1);
        idx = next_idx;
        next_idx = line.find(',', idx+1);
        if (next_idx != std::string::npos) {
            val = std::stoi(line.substr(idx+1, next_idx - idx-1));
        } else{
            val = std::stoi(line.substr(idx+1, line.size() - idx-1));
        }
        idx = next_idx;
        if (item == "children") {
            aunt.children = val;
        } else if (item == "cats") {
            aunt.cats = val;
        } else if (item == "goldfish") {
            aunt.goldfish = val;
        } else if (item == "trees") {
            aunt.trees = val;
        } else if (item == "cars") {
            aunt.cars = val;
        } else if (item == "perfumes") {
            aunt.perfumes = val;
        } else if (item == "samoyeds") {
            aunt.dogs["samoyeds"] = val;
        } else if (item == "pomeranians") {
            aunt.dogs["pomeranians"] = val;
        } else if (item == "akitas") {
            aunt.dogs["akitas"] = val;
        } else if (item == "vizslas") {
            aunt.dogs["vizslas"] = val;
        } else {
            std::cout << "This should not happen" << '\n';
            std::cout << item << '\n';
            exit(0);
        }
    }
    return aunt;
}



int main(int argc, char * argv[]) {
    std::string input = "../input/day_16_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    Sue known_values;
    known_values.children = 3;
    known_values.cats = 7;
    known_values.dogs["samoyeds"] = 2;
    known_values.dogs["pomeranians"] = 3;
    known_values.dogs["akitas"] = 0;
    known_values.dogs["vizslas"] = 0;
    known_values.goldfish = 5;
    known_values.trees = 3;
    known_values.cars = 2;
    known_values.perfumes = 1;
    while(std::getline(file, line)) {
        const auto aunt = parse(line);
        if (aunt.children && aunt.children != known_values.children) continue;
        if (aunt.cats && aunt.cats != known_values.cats) continue;
        if (aunt.goldfish && aunt.goldfish != known_values.goldfish) continue;
        if (aunt.trees && aunt.trees != known_values.trees) continue;
        if (aunt.cars && aunt.cars != known_values.cars) continue;
        if (aunt.perfumes && aunt.perfumes != known_values.perfumes) continue;
        if (aunt.dogs.count("samoyeds") && aunt.dogs.at("samoyeds") != known_values.dogs.at("samoyeds")) continue;
        if (aunt.dogs.count("pomeranians") && aunt.dogs.at("pomeranians") != known_values.dogs.at("pomeranians")) continue;
        if (aunt.dogs.count("akitas") && aunt.dogs.at("akitas") != known_values.dogs.at("akitas")) continue;
        if (aunt.dogs.count("vizslas") && aunt.dogs.at("vizslas") != known_values.dogs.at("vizslas")) continue;
        std::cout << aunt.id << '\n';
        break;
    }
    return 0;
}