#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  std::vector<int> compare(line.size());
  for (int i = 0; i < line.size(); i++) {
    compare[i] = line[i] - '0';
  }

  std::size_t e1 = 0;
  std::size_t e2 = 1;
  std::vector<int> scores{3, 7};
  std::size_t ans = 0;
  std::size_t start_index = 0;
  std::size_t count = 0;
  while (true) {
    const int total_score = scores[e1] + scores[e2];
    if (total_score > 9) {
      scores.push_back(total_score / 10);
    }
    scores.push_back(total_score % 10);
    e1 = (e1 + scores[e1] + 1) % scores.size();
    e2 = (e2 + scores[e2] + 1) % scores.size();
    while (count == 0 && start_index < scores.size()) {
      if (scores[start_index] == compare[0]) {
        count++;
        break;
      }
      start_index++;
    }
    while (start_index + count < scores.size() && count != 0 && count < compare.size()) {
      if (scores[start_index + count] != compare[count]) {
        count = 0;
        start_index++;
        break;
      }
      count++;
    }
    if (count == compare.size()) {
      ans = start_index;
      break;
    }
  }
  std::cout << ans << '\n';
  return 0;
}
