#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_17_input" ;  
  std::ifstream file(input);
  std::string line;
  std::getline(file, line); 
  int current_value = 0;
  int current_position = 0;
  const int n_jumps = std::stoi(line);
  for (int i = 1 ; i <= 50000001; i++) {
    // Not current_position = (current_position + n_jumps + 1) % i;
    current_position = (current_position + n_jumps) % i + 1;
    if (current_position == 1) {
      current_value = i;
    }
  }
  std::cout << current_value << '\n';
  return 0;
}