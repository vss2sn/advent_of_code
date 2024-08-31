#include <fstream>
#include <iostream>
#include <regex> 
#include <string>
#include <string_view>
#include <unordered_map> 
#include <unordered_set> 
#include <utility>
#include <vector>

int main(int argc, char * argv[]) {
    std::string input = "../input/day_19_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::vector<std::pair<std::string, std::string>> replacements;
    const std::regex pattern(R"(([A-Za-z]+) => ([A-Za-z]+))");
    while(std::getline(file, line)) {
        if (line.empty()) break;
        std::smatch match;
        std::regex_search(line, match, pattern);
        replacements.emplace_back(match[1], match[2]);
    }
    std::getline(file, line);
    std::unordered_set<std::string> new_molecules;
    const std::string_view medicine_molecule = line;
    for (const auto& [key, value] : replacements) {
        for (int i = 0; i < medicine_molecule.size(); i++) {
            if (i + key.size() - 1 >= medicine_molecule.size()) continue;
            std::string_view sub_mm = medicine_molecule.substr(i, key.size());
            if (key == sub_mm) {
                const auto new_molecule = ((i > 0) ? std::string(medicine_molecule.substr(0, i)) : std::string(""))
                    + value 
                    + ((i == medicine_molecule.size() - 1) ? std::string("") : std::string(medicine_molecule.substr(i + key.size(), medicine_molecule.size() - i - key.size())));
                new_molecules.insert(new_molecule);
            }
        }
    }
    std::cout << new_molecules.size() << '\n';
    return 0;
}
