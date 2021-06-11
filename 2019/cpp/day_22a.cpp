#include <fstream>
#include <iostream>
#include <string>

constexpr int n_cards = 10007;
constexpr int start_position = 2019;

int64_t mod(const int64_t a, const int64_t b) {
  return (a >= 0) ? (a % b) : (b - (-a) % b);
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;

  int A = 1;
  int B = 0;
  while(std::getline(file, input_line)) {
    int a, b;
    if (input_line.substr(0,19) == "deal with increment") {
      a = std::stoi(input_line.substr(19, input_line.size() - 19));
      b = 0;
    } else if (input_line.substr(0,19) == "deal into new stack") {
      a = -1;
      b = n_cards - 1;
    } else if (input_line.substr(0,3) == "cut") {
      auto n_cut = std::stoi(input_line.substr(3, input_line.size() - 3));
      if (n_cut < 0) {
        n_cut += n_cards;
      }
      a = 1;
      b = n_cards - n_cut;
    }
    A = mod(a * A, n_cards);
    B = mod(a * B + b, n_cards);
  }

  int64_t current_position = start_position;
  // Shuffle once
  current_position = mod(A * current_position + B, n_cards);

  std::cout << current_position << '\n';
  return current_position;
}
