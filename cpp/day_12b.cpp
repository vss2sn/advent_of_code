#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::ifstream file{"day_12_data.txt"};
  std::string com_inst;
  int ship_pos_x = 0;
  int ship_pos_y = 0;
  int wayp_pos_x = 10;
  int wayp_pos_y = 1;
  while(std::getline(file, com_inst)) {
    char inst = com_inst[0];
    const int mag = std::stoi(com_inst.substr(1, com_inst.size() - 1));
    if (inst == 'F') {
      ship_pos_x += mag * wayp_pos_x;
      ship_pos_y += mag * wayp_pos_y;
    } else if(inst == 'N') {
        wayp_pos_y += mag;
    } else if (inst == 'S') {
        wayp_pos_y -= mag;
    } else if (inst == 'E') {
        wayp_pos_x += mag;
    } else if (inst == 'W') {
        wayp_pos_x -= mag;
    } else if (inst == 'L') {
      int dir = mag / 90;
      dir += 4;
      dir %= 4;
      for(int i = 0; i < dir; ++i) {
        const int temp = wayp_pos_x;
        wayp_pos_x = -wayp_pos_y;
        wayp_pos_y = temp;
      }
    } else if (inst == 'R') {
      int dir = mag / 90;
      dir += 4;
      dir %= 4;
      for(int i = 0; i < dir; ++i) {
        const int temp = wayp_pos_x;
        wayp_pos_x = wayp_pos_y;
        wayp_pos_y = -temp;
      }
    } else {
      std::cout << "This should also not happen" << '\n';
    }
    // std::cout << "(" << ship_pos_x << ", " << ship_pos_y << ")" << '\n';
  }
  int manh_dist =std::abs(ship_pos_x) + std::abs(ship_pos_y);
  std::cout << manh_dist << '\n';
  return manh_dist;
}
