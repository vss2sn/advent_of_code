#include <fstream>
#include <iostream>
#include <string>

void move (const std::size_t& start, const std::size_t& end, int& x, int& y, int& direction, const std::string& instructions) {
    if (instructions[start] == 'L') {
        direction--;
    } else {
        direction++;
    }
    if(direction < 0) {
        direction += 4;
    }
    direction %= 4;
    // Instead of using substr and then stoi can use this:
    int num = 0;
    for (std::size_t idx = start+1; idx < end; idx++) {
        num = num * 10 + (instructions[idx] - '0');
    }
    if (direction == 0) {
        y+=num;
    }
    else if (direction == 1) {
        x+=num;
    }
    else if (direction == 2) {
        y-=num;
    }
    else  {
        x-=num;
    }
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string instructions;
  std::getline(file, instructions);
  int x = 0;
  int y = 0;
  int direction = 0;
  std::size_t start = 0;
  const std::string delimiter = ", ";
  std::size_t end = instructions.find(delimiter, start);
  while(end != std::string::npos) {
    move(start, end, x, y, direction, instructions);
    start = end + delimiter.size();
    end = instructions.find(delimiter, start);
  }
  move(start, instructions.size(), x, y, direction, instructions);
  std::cout << std::abs(x) + std::abs(y) << '\n';
  return 0;
}
