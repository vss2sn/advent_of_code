#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::ifstream file{"day_12_data.txt"};
  std::string com_inst;
  int dir = 0;
  int pos_x = 0;
  int pos_y = 0;
  while(std::getline(file, com_inst)) {
    char inst = com_inst[0];
    const int mag = std::stoi(com_inst.substr(1, com_inst.size() - 1));
    if (inst == 'F') {
      if(dir == 0) inst = 'E';
      else if(dir == 1) inst = 'N';
      else if(dir == 2) inst = 'W';
      else if(dir == 3) inst = 'S';
      else {
        std::cout << dir <<" This should not happen" << '\n';
      }
    }

    if(inst == 'N') {
        pos_y += mag;
    } else if (inst == 'S') {
        pos_y -= mag;
    } else if (inst == 'E') {
        pos_x += mag;
    } else if (inst == 'W') {
        pos_x -= mag;
    } else if (inst == 'L') {
        dir += mag / 90;
        dir %= 4;
    } else if (inst == 'R') {
      dir -= mag / 90;
      dir += 4;
      dir %= 4;
    } else {
      std::cout << "This should also not happen" << '\n';
    }
  }
  std::cout << std::abs(pos_x) + std::abs(pos_y) << '\n';
  return 0;
}
