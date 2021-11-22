#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  int n = stoi(line);
  const int n_recipes = n + 11;
  int e1 = 0;
  int e2 = 1;
  std::vector<int> scores{3, 7};
  while (scores.size() < n_recipes) {
    int total_score = scores[e1] + scores[e2];
    if (total_score > 9) {
      scores.push_back(total_score / 10);
    }
    scores.push_back(total_score % 10);
    e1 = (e1 + scores[e1] + 1) % scores.size();
    e2 = (e2 + scores[e2] + 1) % scores.size();
  }

  long long ans = 0;
  for (int i = n_recipes - 11; i < n_recipes - 1; i++) {
    ans = ans * 10 + scores[i];
  }
  std::cout << ans << '\n';
  return 0;
}
