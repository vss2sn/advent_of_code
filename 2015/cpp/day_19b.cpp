#include <fstream>
#include <iostream>
#include <limits>
#include <regex> 
#include <string>
#include <string_view>
#include <unordered_map> 
#include <unordered_set> 
#include <utility>
#include <vector>

// On checking the inputs it's clear that Ca reduces to nothing as CaX or XCa where C is any string reduces to X.
// So increment the number of operations performed each time a Ca is found and do not bother adding it to the new molecule created. 

int reduce (const std::string & input, 
    const std::vector<std::pair<std::string, std::string>>& replacements, 
    int n_replacements,
    int& min_replacements,
    std::unordered_map<std::string, int>& worked) {
    if (n_replacements > min_replacements) return std::numeric_limits<int>::max();
    if (input == "e") {
        min_replacements = std::min(n_replacements, min_replacements);
        std::cout <<min_replacements << '\n';
        exit(0); // Early return
        return 1;
    }
    int temp_n = std::numeric_limits<int>::max();
    for (const auto& [reduces_to, key] : replacements) {
        for (int i = 0; i < input.size(); i++) {
            if (i + key.size() - 1 >= input.size()) continue;
            std::string_view sub_mm = input.substr(i, key.size());
            if (key == sub_mm) {
                const auto new_molecule = ((i > 0) ? std::string(input.substr(0, i)) : std::string(""))
                    + ((reduces_to == "Ca") ? "" : reduces_to)
                    + ((i == input.size() - 1) ? std::string("") : std::string(input.substr(i + key.size(), input.size() - i - key.size())));
                if (reduces_to == "Ca") n_replacements++;
                if (worked.count(new_molecule) != 0) {
                    if (worked[new_molecule] == std::numeric_limits<int>::max()) continue;
                    min_replacements = std::min(n_replacements + worked[new_molecule], min_replacements);
                    continue;
                }
                if (reduces_to == "e" && new_molecule.size() > 1) {
                    if (reduces_to == "Ca") n_replacements--;
                    continue;
                }
                const int final_n = reduce (new_molecule, replacements, n_replacements+1, min_replacements, worked);
                if (final_n == std::numeric_limits<int>::max()) {
                    worked[new_molecule] = std::numeric_limits<int>::max();
                } else {
                    worked[new_molecule] = final_n + 1;
                    temp_n = std::min(worked[new_molecule], temp_n);
                }   
                if (reduces_to == "Ca") n_replacements--;
            }
        }
    }
    return temp_n;
}

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
    std::string new_line;
    int n_Ca_replacements = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == 'C' && line[i+1] == 'a') {
            i += 1;
            n_Ca_replacements++;
        } else {
            new_line += line[i];
        }
    }
    int min_replacements = std::numeric_limits<int>::max();
    std::unordered_map<std::string, int> worked;
    reduce(new_line, replacements, n_Ca_replacements, min_replacements, worked);
    std::cout << min_replacements << '\n';
    return 0;
}
