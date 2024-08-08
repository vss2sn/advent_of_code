#include <fstream>
#include <iostream>
#include <string>

// Make sure that the line ending are accurate (let to LF instead of CRLF)

int parse_line_and_get_delta(const std::string& line) {
    std::string just_chars = "";
    for (int i = 1; i < line.size()-1; i++) {
        if (line[i] != '\\') {
            just_chars += line[i];
        }        
        else if (line[i+1] == 'x') {
            std::string hex_value = line.substr(i + 2, 2);    
            int num = std::stoi(hex_value, 0, 16);
            char c = char(num);
            just_chars += (c);
            i+=3;
        } else {
            just_chars += line[i+1];
            i++;
        }
    }
    // std::cout << line << '\n';
    // std::cout << just_chars << '\n';
    return line.size() - just_chars.size();
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  int total = 0;
  while(std::getline(file, line)) {
    total += parse_line_and_get_delta(line);
  }
  std::cout << total << '\n';
  return 0;
}