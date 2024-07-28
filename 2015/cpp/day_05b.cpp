#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map> 
#include <vector>  

bool is_forbidden(const char c1, const char c2) {
    if (c1 == 'a' && c2 == 'b') return true;
    if (c1 == 'c' && c2 == 'd') return true;
    if (c1 == 'p' && c2 == 'q') return true;
    if (c1 == 'x' && c2 == 'y') return true;
    return false;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_05_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  int nice_count = 0;
  while(std::getline(file, line)) {
    int vowel_count = 0;
    bool contains_repeated = false;
    std::unordered_map<std::string, std::vector<int>> indices_of_pairs;
    for (int i = 0; i < line.size() - 2; i++) {
        if (line[i] == line[i+2]) {
            contains_repeated = true;
        }
        indices_of_pairs[std::string(line.begin() + i, line.begin() + i + 2)].push_back(i);
    }
    indices_of_pairs[std::string(line.begin() + line.size() - 2, line.begin() + line.size())].push_back(line.size() - 2);
    if (!contains_repeated) continue;
    bool counts = false;
    for (const auto& [sv, indices] : indices_of_pairs) {    
        if (indices.size() < 2) continue; 
        counts = true;
        for (int i = 0; i < indices.size() - 1; i++) {
            if (indices[i] + 1 == indices[i+1]) {
                counts = false;
            }
        }
        if (counts) break;
    }
    if (!counts) continue;
    nice_count++;
  }
  std::cout << nice_count << '\n';
  return 0;
}
