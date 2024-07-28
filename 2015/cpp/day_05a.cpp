#include <fstream>
#include <iostream>
#include <string>
#include <vector>  

bool is_vowel (const char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

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
    bool contains_forbidden = false;
    for (int i = 0; i < line.size() - 1; i++) {
        if (is_vowel(line[i])) {
            vowel_count++;
        }
        if (line[i] == line[i+1]) {
            contains_repeated = true;
        }
        if (is_forbidden(line[i], line[i+1])) {
            contains_forbidden = true;
            break;
        }
    }
    if (is_vowel(line[line.size() - 1])) vowel_count++;
    if (vowel_count>=3 &&contains_repeated && !contains_forbidden) {
        nice_count++;
    }
  }
  std::cout << nice_count << '\n';
  return 0;
}
