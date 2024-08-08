#include <fstream>
#include <iostream>
#include <string>

// Make sure that the line ending are accurate (let to LF instead of CRLF)

int parse_line_and_get_delta(const std::string& line) {
    std::string just_chars = "";
    for (int i = 0; i < line.size(); i++) {
        if (std::isalnum(line[i])) {
            just_chars += line[i];
        } else {
            just_chars += "\\" + std::string{line[i]};
        }
    }
    just_chars = '\"' + just_chars + '\"';
    return  just_chars.size() - line.size();
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