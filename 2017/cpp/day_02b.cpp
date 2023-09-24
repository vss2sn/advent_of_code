#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_02_input" ;  
  std::ifstream file(input);
  std::string line;
  int checksum = 0; 
  while(std::getline(file, line)) { 
    std::vector<int> numbers;
    const std::string delimiter = " ";
    size_t start = 0;
    size_t end = line.find(delimiter);
    while (end < std::string::npos) {
        numbers.emplace_back(std::stoi(line.substr(start, end - start)));
        start = end + delimiter.size();
        end = line.find(delimiter, start);
    }
    numbers.emplace_back(std::stoi(line.substr(start, line.size() - start)));
    
    bool found = false;
    for (int i = 0; i < numbers.size() && !found; i++) {
        for (int j = i + 1; j < numbers.size() && !found; j++) {
            if (numbers[j] % numbers[i] == 0) {
                found = true;
                checksum += numbers[j] / numbers[i];
            }
            if ( numbers[i] %  numbers[j] == 0) {
                found = true;
                checksum +=  numbers[i] / numbers[j];
            }
        }
    }
  }
  std::cout << checksum << '\n';
  return 0;
}
