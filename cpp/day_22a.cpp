#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <deque>
#include <functional>

int main() {
  std::fstream file{"../input/day_22_input"};
  std::string line;
  std::vector<std::deque<int>> player_decks;
  std::vector<int> players;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](const char c) {return !isprint(c);}), std::end(line));
    if(line == "") continue;
    if(line.substr(0, 6) == "Player") {
      player_decks.emplace_back();
      players.push_back(std::stoi(line.substr(7, line.size() - 8)));
    } else {
      player_decks.back().push_back(std::stoi(line));
    }
  }
  int total_players = players.size();

  // Play
  while (player_decks.size() > 1) {
    size_t winning_player = -1;
    int hightest_card = -1;
    std::vector<int> top_cards(player_decks.size());
    for(int i = 0; i < player_decks.size(); i++) {
      if(hightest_card < player_decks[i].front() ) {
        hightest_card = player_decks[i].front();
        winning_player = i;
      }
      top_cards[i] = player_decks[i].front();
      player_decks[i].pop_front();
    }
    std::sort(std::begin(top_cards), std::end(top_cards), std::greater<int>());
    for(const auto& top_card : top_cards) {
      player_decks[winning_player].push_back(top_card);
    }
    for(int i = 0; i < player_decks.size(); i++) {
      if(player_decks[i].empty()) {
        player_decks.erase(std::begin(player_decks) + i);
        players.erase(std::begin(players) + i);
        i--;
      }
    }
  }

  long long score = 0;
  for (const auto& player_deck : player_decks) {
    for(int i = 0; i < player_deck.size(); i++) {
      score += ((i+1) * player_deck[player_deck.size() - 1 - i]);
    }
  }
  std::cout << score << '\n';
  return score;
}
