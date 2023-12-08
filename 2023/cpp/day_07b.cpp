#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_map>

struct Hand {
  std::string cards;
  std::array<int, 5> label_values;
  int type;
};

constexpr std::array<char, 13> labels {{'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'}};

int get_type(const std::string& cards) {
  std::unordered_map<char, int> m;
  for (const auto card : cards) {
    if (m.find(card) == m.end()) m[card] = 0;
    m[card]++; 
  }

  for (const auto & [card , n] : m) {
    std::cout << "(" << card << ',' << n << ")" << ' ';
  }
  std::cout << '\n';
  if (m.size() == 1) {
    return 6;
  }
  if (m.size() == 2 && (m['J'] != 0)) {
    return 6;
  }

  if (m.size() == 2 && (m[cards[0]] == 4 || m[cards[0]] == 1)) {
    return 5;
  }
  if (m.size() == 3) {
    for (const auto& [card, n] : m) {
      if (card =='J') continue;
      if (n + m['J'] == 4) return 5;
    }
  }

  if (m.size() == 2 && (m[cards[0]] == 3 || m[cards[0]] == 2)) {
    return 4;
  }
  if (m.size() == 3) {
    for (const auto& [card, n] : m) {
      if (card =='J') continue;
      if (n + m['J'] == 3) return 4;
    }
  }

  int count_pairs = 0;
  int count_three_of_a_kind = 0;
  int single = 0;
  for (const auto& [card, n] : m) {
    if (card == 'J') continue;
    if (n == 1) single++;
    if (n == 2) count_pairs++;
    if (n == 3) count_three_of_a_kind++;
  }
  if (count_three_of_a_kind == 1) return 3; 
  for (const auto [card, n] : m) {
    if (card =='J') continue;
    if (n + m['J'] == 3) return 3;
  }
  if (count_pairs > 0 && m['J'] > 0) return 3;  
  if (count_pairs == 2) return 2;
  if (count_pairs + std::min(single, m['J']) == 2) return 2;  
  if (count_pairs == 1) return 1;
  for (const auto [card, n] : m) {
    if (card =='J') continue;
    if (n + m['J'] == 2) return 1;
  }
  return 0;
}

int get_value(const char label) {
  return std::distance(std::begin(labels), std::find(std::begin(labels), std::end(labels), label));
}

std::pair<Hand, int> get_hand_and_bid(const std::string& line) {
  std::cout << line << '\n';
  const auto idx = line.find(' ');
  Hand hand;
  hand.cards = line.substr(0, idx);
  std::cout << hand.cards << '|' << '\n';
  hand.type = get_type(hand.cards);
  std::cout << hand.type << '\n';
  for (int i = 0; i < hand.cards.size(); i++) {
    hand.label_values[i] = get_value(hand.cards[i]);
    std::cout << hand.label_values[i] << ' ';
  }
  std::cout << '\n';

  return std::make_pair(hand, std::stoi(line.substr(idx, line.size() - idx)));
}

bool compare_hands(const Hand& hand1, const Hand& hand2) {
  if (hand1.type > hand2.type) {
    return true;
  } else if (hand1.type < hand2.type) {
    return false;
  }
  for (int i = 0; i < 5; i++) {
    if (hand1.label_values[i] > hand2.label_values[i]) return true;
    if (hand1.label_values[i] < hand2.label_values[i]) return false;
  }
  return true;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::pair<Hand, int>> hands_and_bids;
  while(std::getline(file, line)) {
    hands_and_bids.emplace_back(get_hand_and_bid(line));
  }

  std::sort(std::begin(hands_and_bids), 
            std::end(hands_and_bids), 
            [](const auto& hand_and_bid_1, const auto& hand_and_bid_2) { 
              return !compare_hands(hand_and_bid_1.first, hand_and_bid_2.first);
            });

  std::size_t total = 0;
  for (int i = 0; i < hands_and_bids.size(); i++) {
    std::cout << hands_and_bids[i].first.cards << ' ' << hands_and_bids[i].first.type << ' ' << hands_and_bids[i].second << '\n';
    total += (i+1) * hands_and_bids[i].second;
  }
  std::cout << total << '\n';
  return 0;
}