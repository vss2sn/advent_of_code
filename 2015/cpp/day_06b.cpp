#include <fstream>
#include <iostream>
#include <string>
#include <vector>  
#include <array>  

int main(int argc, char* argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::array<std::array<int, 1000>, 1000> lights;
  for (auto& row : lights) {
      for (auto& col : row) {
          col = 0;
      }   
  }
  
  while(std::getline(file, line)) {
    std::vector<std::string> split;
    const std::string delimiter = " ,";
    size_t start = 0;
    size_t end = line.find_first_of(delimiter);
    while (end != std::string::npos) {
      split.push_back(line.substr(start, end - start));
      start = end + 1;
      end = line.find_first_of(delimiter, start);
    }
    split.push_back(line.substr(start, line.size() - start));
    if (split[0] == "turn" && split[1] == "on") {
        for (int row = std::stoi(split[2]); row <= std::stoi(split[5]); row++) {
            for (int col = std::stoi(split[3]); col <= std::stoi(split[6]); col++) {
                lights[row][col] += 1;
            }    
        }
    }
    if (split[0] == "turn" && split[1] == "off") {
        for (int row = std::stoi(split[2]); row <= std::stoi(split[5]); row++) {
            for (int col = std::stoi(split[3]); col <= std::stoi(split[6]); col++) {
                lights[row][col] = std::max(lights[row][col] - 1, 0);
            }    
        }
    }
    if (split[0] == "toggle") {
        for (int row = std::stoi(split[1]); row <= std::stoi(split[4]); row++) {
            for (int col = std::stoi(split[2]); col <= std::stoi(split[5]); col++) {
                lights[row][col] += 2;
            }    
        }
    }
  }
  int brightness = 0;
  for (const auto& row : lights) {
      for (const auto& col : row) {
          brightness += col;
      }   
  }
  std::cout << brightness << '\n';
  return 0;
}