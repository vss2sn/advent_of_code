#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char * argv[]) {

  std::string input = "../input/day_21_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  int p1 = line[28] - '0';
  std::getline(file, line);
  int p2 = line[28] - '0';

  int score1 = 0;
  int score2 = 0;
  int n_move = 6;
  int count = 0;
  int rolls = 0;
  while (true) {
    p1 += n_move;
    p1 = p1 % 10; // let 10 be position 0;
    score1 += (p1 == 0 ? 10 : p1); // ensure points added with 10
    rolls += 3;
    if (score1 >= 1000) {
      std::cout << rolls * score2 << '\n';
      return 0;
    }
    n_move += 9;

    p2 += n_move;
    p2 = p2 % 10; // let 10 be position 0;
    score2 += (p2 == 0 ? 10 : p2); // ensure points added with 10
    rolls += 3;
    if (score2 >= 1000) {
      std::cout << rolls * score1 << '\n';
      return 0;
    }
    n_move += 9;
  }
  return 0;
}
