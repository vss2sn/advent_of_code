#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
// TODO(vss): improve the previous_rounds so it can be generalized to n players. Rest already generalized. Multimap?
std::tuple<int, std::vector<std::deque<int>>, std::vector<int>> PlayRecursiceCombat(std::vector<std::deque<int>> player_decks, std::vector<int> players) {
  std::map<std::deque<int>, std::set<std::deque<int>>> previous_rounds;
  while (player_decks.size() > 1) {
    if(auto it = previous_rounds.find(player_decks[0]);
       it != previous_rounds.end() && it->second.find(player_decks[1]) != it->second.end()) {
        return {1, player_decks, players}; // player 1 wins game
    } else {
      previous_rounds[player_decks[0]].insert(player_decks[1]);
    }

    size_t winning_player = -1;
    int hightest_card = -1;
    std::vector<int> top_cards(player_decks.size());
    bool n_remaining_greater_than_card_value = true;
    for(int i = 0; i < player_decks.size(); i++) {
      if(hightest_card < player_decks[i].front() ) {
        hightest_card = player_decks[i].front();
        winning_player = players[i];
      }
      if(player_decks[i].front() > player_decks[i].size() - 1) {
        n_remaining_greater_than_card_value = false;
      }
      top_cards[i] = player_decks[i].front();
      player_decks[i].pop_front();
    }

    if(n_remaining_greater_than_card_value) {
      std::vector<std::deque<int>> subgame_player_decks;
      subgame_player_decks.reserve(top_cards.size());
      for(int i = 0; i < top_cards.size(); i++) {
        subgame_player_decks.emplace_back(std::begin(player_decks[i]), std::begin(player_decks[i]) + top_cards[i]);
      }
      auto result = PlayRecursiceCombat(subgame_player_decks, players);
      winning_player = std::get<0>(result);
    }
    auto dist = std::distance(std::begin(players), std::find(std::begin(players), std::end(players), winning_player));
    std::rotate(std::begin(top_cards), std::begin(top_cards) + dist, std::end(top_cards));

    if(std::find(std::begin(players), std::end(players), winning_player) == std::end(players)) {
      players.emplace_back(winning_player);
      player_decks.emplace_back();
    }

    for(const auto& top_card : top_cards) {
      auto dist = std::distance(std::begin(players), std::find(std::begin(players), std::end(players), winning_player));
      player_decks[dist].push_back(top_card);
    }

    for(int i = 0; i < player_decks.size(); i++) {
      if(player_decks[i].empty()) {
        player_decks.erase(std::begin(player_decks) + i);
        players.erase(std::begin(players) + i);
        i--;
      }
    }
  }
  return {players[0], player_decks, players};
}

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
  auto result = PlayRecursiceCombat(player_decks, players);
  auto dist = std::distance(std::begin(std::get<2>(result)),
    std::find(std::begin(std::get<2>(result)), std::end(std::get<2>(result)), std::get<0>(result)));
  long long score = 0;
  const auto& deck = std::get<1>(result)[dist];
  for(int i = 0; i < deck.size(); i++) {
    score += ((i+1) * deck[deck.size() - 1 - i]);
  }
  std::cout << score << '\n';
  return score;
}
