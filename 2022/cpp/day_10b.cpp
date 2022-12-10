#include <array>
#include <fstream>
#include <iostream>
#include <string>

class CommSystem {
public:
  void noop() {
    tick();
  };

  void addx(const int val) {
    tick();
    tick();
    X += val;
  }

  long long get_signal_strength() {
    return signal_strength;
  }

  void print_screen() {
    for(const auto& row : screen) {
      for (const auto ele : row) {
        std::cout << ele;
      }
      std::cout << '\n';
    }
  }


private:

  void update_crt() {
    const auto pixel_id = (cycle_count  - 1) % 240;
    const auto pixel_row = pixel_id / 40;
    const auto pixel_col = pixel_id % 40;
    // std::cout << "CRT at position: " << pixel_id << " (" << pixel_row << ',' << pixel_col << ')' << '\n';
    if (pixel_col == X || pixel_col == (X - 1) || pixel_col == (X + 1)) {
      screen[pixel_row][pixel_col] = '#';
    } else {
      screen[pixel_row][pixel_col] = '.';
    }
  }

  void tick() {
    // std::cout << "Begin cycle: " << cycle_count + 1 << '\n';
    cycle_count++;
    update_crt();
    // print_screen();
    if ((cycle_count - 20) % 40 == 0) {
      signal_strength += cycle_count * X;
    }
    // std::cout << "End cycle: " << cycle_count << '\n';
  }

  long long X = 1;
  long long cycle_count = 0;
  long long signal_strength = 0;
  std::array<std::array<char, 40>, 6> screen = {};
};


int main(int argc, char * argv[]) {
  std::string input = "../input/day_10_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  CommSystem cs;

  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    const auto space = line.find(' ');
    const auto instr = line.substr(0, space);
    if (instr == "noop") {
      cs.noop();
    } else if (instr == "addx") {
      cs.addx(std::stoi(line.substr(space + 1, line.size() - space - 1)));
    }
  }

  cs.print_screen();

  return 0;
}
