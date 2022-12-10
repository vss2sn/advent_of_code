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

private:

  void tick() {
    cycle_count++;
    if ((cycle_count - 20) % 40 == 0) {
      signal_strength += cycle_count * X;
    }
  }

  long long X = 1;
  long long cycle_count = 0;
  long long signal_strength = 0;
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

  std::cout << cs.get_signal_strength() << '\n';

  return 0;
}
