#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_04_input" ;  
  std::ifstream file(input);
  std::string line;
  int n_valid = 0;
  while(std::getline(file, line)) {
    bool valid = true;
    std::size_t start = 0;
    std::size_t end = line.find(' ', start);
    std::unordered_set<std::string> words;
    while(end != std::string::npos) {
        const auto word = line.substr(start, end - start);
        std::cout << '|' << word << '|' << '\n';
        if(!words.insert(word).second) {
            std::cout << "Duplicate word " << word << " detected. Invalid passphrase." << '\n';
            valid = false;
            break;
        }
        start = end + 1;
        end = line.find(' ', start);
    }
    if (valid) {
        const auto word = line.substr(start, line.size() - start);
        std::cout << '|' << word << '|' << '\n';
        if(!words.insert(word).second) {
            std::cout << "Duplicate word " << word << " detected. Invalid passphrase." << '\n';
            valid = false;
        }
    }
    
    if (valid) {
        std::cout << "Valid passphrase." << '\n';
        n_valid++;
    }
  }
  std::cout << "Number of valid passphrases: " << n_valid << '\n';
  return 0;
}