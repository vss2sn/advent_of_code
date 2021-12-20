#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

// Debug
// #include <cassert>
// #include <chrono>
// #include <thread>
// using namespace std::literals::chrono_literals;

struct hash_quad {
  size_t operator()(const std::array<int, 4>& p) const {
    size_t hash = 0;
    for (const auto& ele : p) {
      // hash << 8;
      // hash += ele;
      hash = hash * 100 + ele;
    }
    return hash;
  }
};

std::unordered_map<int, long long> freq = {
  {3, 1},
  {4, 3},
  {5, 6},
  {6, 7},
  {7, 6},
  {8, 3},
  {9, 1}
};

// Returns the number of games won by players. Index of array = player
std::array<long long, 2> play(
  const long long player,
  std::array<int, 2>& scores,
  std::array<int, 2>& positions,
  const int current_roll,
  std::unordered_map<std::array<int, 4>, std::pair<long long, long long>, hash_quad>& results,
  const int depth = 0) {

  // Sanity check; depth not required for actual algo
  // assert(depth <= 42);

  const auto prev_score = scores[player];
  const auto prev_position = positions[player];
  const auto next_player = (player + 1) % 2;

  positions[player] += current_roll;
  positions[player] %= 10;
  scores[player] += positions[player] + 1;

  // Number of wins by each player in this branch
  std::array<long long, 2> ret_val = {{0,0}};

  // Key (state) as described in main function
  const auto p = std::array<int, 4>{{positions[player], positions[next_player], scores[player], scores[next_player]}};
  if (const auto it = results.find(p); it != results.end()) {
    scores[player] = prev_score;
    positions[player] = prev_position;
    ret_val[player] = it->second.first;
    ret_val[next_player] = it->second.second;
    return ret_val;
  }

  if (scores[player] >= 21) {
    scores[player] = prev_score;
    positions[player] = prev_position;
    ret_val[player] = 1;
    return ret_val;
  }

  for (int c_roll = 3; c_roll <= 9; c_roll++) {
    const auto temp = play(next_player, scores, positions, c_roll, results, depth+1);
    ret_val[player] += (temp[player] * freq[c_roll]);
    ret_val[next_player] += (temp[next_player] * freq[c_roll]);
  }

  // assert(results.find(p) == results.end());
  results[p] = {ret_val[player], ret_val[next_player]};

  scores[player] = prev_score;
  positions[player] = prev_position;
  return ret_val;
}


int main(int argc, char * argv[]) {
  std::string input = "../input/day_21_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  // Position of the players 1 in this branch of the game, ie this universe
  std::array<int, 2> positions;

  // Score of the players in this branch of the game, ie this universe
  std::array<int, 2> scores = {{0,0}};

  // Cache intermediate results
  // For a pair (key, value) where key = a, b, c, d and value = e, f
  // a is the position of a player (p) who just rolled the dice 3 times
  // b is the position of the other player (op)
  // c = p's score
  // d = op's score
  // p's position (a) and score (c) have been updated for the current roll
  // the number of games p will win is e and the number of games op will win is f
  std::unordered_map<std::array<int, 4>, std::pair<long long, long long>, hash_quad> results; // Cache intermediate results

  std::getline(file, line);
  positions[0] = line[28] - '1';
  std::getline(file, line);
  positions[1] = line[28] - '1';

  std::array<long long, 2> n_wins = {{0,0}};
  for (int c_roll = 3; c_roll <= 9; c_roll++) {
    const auto temp = play(0, scores, positions, c_roll, results, 0);;
    n_wins[0] += (temp[0] * freq[c_roll]);
    n_wins[1] += (temp[1] * freq[c_roll]);
  }
  // std::cout << n_wins[0] << ' ' << n_wins[1] << '\n';

  std::cout << std::max(n_wins[0], n_wins[1]) << '\n';
  return 0;
}
