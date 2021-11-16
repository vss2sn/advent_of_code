#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <memory>
#include <numeric>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  size_t start = 0;
  size_t end = line.find(" ");
  std::vector<std::string> parsed_input;
  while(end != std::string::npos) {
    parsed_input.push_back(line.substr(start, end - start));
    start = end + 1;
    end = line.find(" ", start);
  }
  parsed_input.push_back(line.substr(start, line.size() - start - 1));

  const long long n_players = stoi(parsed_input[0]);
  const long long n_marbles = stoi(parsed_input[6]) * 100;
  std::vector<long long> scores(n_players, 0);
  std::vector<long long> next_marbles(n_marbles+1, 0);
  std::vector<long long> prev_marbles(n_marbles+1 , 0);
  long long c_marble = 0;
  long long next_lowest_marble = 1;
  long long turn = 0;

  while (next_lowest_marble <= n_marbles) {
    turn = turn % n_players;
    if (next_lowest_marble % 23 != 0) {
      next_marbles[next_lowest_marble] = next_marbles[next_marbles[c_marble]];
      prev_marbles[next_lowest_marble] = next_marbles[c_marble];
      prev_marbles[next_marbles[next_marbles[c_marble]]] = next_lowest_marble;
      next_marbles[next_marbles[c_marble]] = next_lowest_marble;
      c_marble = next_lowest_marble;
    } else {
      scores[turn] += next_lowest_marble;
      long long prev_marble = c_marble;
      for (long long i = 0; i < 7; i++) {
        prev_marble = prev_marbles[prev_marble];
      }
      scores[turn] += prev_marble;
      const auto pp_marble = prev_marbles[prev_marble];
      const auto np_marble = next_marbles[prev_marble];
      next_marbles[pp_marble] = np_marble;
      prev_marbles[pp_marble] = pp_marble;
      c_marble = np_marble;
    }
    next_lowest_marble++;
    turn++;
  }

  const long long max_score = *std::max_element(std::begin(scores), std::end(scores));
  std::cout << max_score << '\n';
  return max_score;
}
