#include <fstream>
#include <iostream>
#include <string>
#include <queue>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::queue<int> window;
  constexpr int n = 3;
  int cur = 0;
  for (int i = 0; i < n; i++) {
    std::getline(file, line);
    const int temp = std::stoi(line);
    window.push(temp);
    cur += temp;
  }
  int prev = cur;
  int count = 0;
  while(std::getline(file, line)) {
    prev = cur;
    cur -= window.front();
    window.pop();
    const int temp = std::stoi(line);
    cur += temp;
    window.push(temp);
    if (cur > prev) {
      count++;
    }
  }
  std::cout << count  << '\n';
  return 0;
}
