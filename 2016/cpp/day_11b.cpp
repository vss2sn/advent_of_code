#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>    
#include <queue>    
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// NOTE: The new elements were added to the input.

enum class Type {
    GENERATOR, MICROCHIP, ELEVATOR
};

struct StateScoreMoves {
  std::vector<int> state;
  int score = 0;
  int moves = 0;
};

struct Comparator {
  bool operator () (const StateScoreMoves& ssm1, const StateScoreMoves& ssm2) const {
    return ssm1.moves > ssm2.moves;
  }
};

struct hash_state {
  std::size_t operator () (const std::vector<int>& state) const {
    int hash = 0;
    for (const auto& ele : state) {
      hash = hash * 10 + ele;
    }
    return hash;
  }
};


std::vector<std::string> elements;// Map element name to id
std::vector<std::pair<Type, int>> col_titles{{Type::ELEVATOR, -1}}; // Index i of state maps to (Type and element id) pair at index i
std::vector<std::pair<int, int>> pairings; // Maps index to where column index of (generator, microchip)
std::unordered_set<std::vector<int>, hash_state> seen;
std::vector<int> change_floor{-1,1}; // Moves 

constexpr int max_floor = 4;
constexpr int min_floor = 1;

bool valid(const std::vector<int>& state, int col) {
  const auto& [type, id] = col_titles[col];
  const auto& [gen_col, chip_col] = pairings[id];
  const auto gen_floor = state[gen_col];
  const auto chip_floor = state[chip_col];
  // Both microchip and generator of the moved item are now at the same level
  const bool chip_safe = (gen_floor == chip_floor);
  // Check whether an unconnected chip exists on gen_floor and a generator exists on chip_floor
  for (const auto& [ele_gen_col, ele_chip_col] : pairings) {
    const auto ele_gen_floor = state[ele_gen_col];
    const auto ele_chip_floor = state[ele_chip_col];
    const auto ele_chip_safe = (ele_gen_floor == ele_chip_floor);
    if (!chip_safe && ele_gen_floor == chip_floor) {
      return false; // If true then ele_gen will fry the chip
    }
    if (!ele_chip_safe && ele_chip_floor == gen_floor) {
      return false; // If true then gen will fry the ele_chip
    }
  }
  return true;
}

std::vector<std::vector<int>> get_next_states(const std::vector<int>& state) {
  std::vector<std::vector<int>> new_states;
  const auto ele_floor = state.at(0);
  for (const auto& delta : change_floor) {
    auto new_state = state;
    new_state[0] += delta;
    if (new_state[0] > max_floor || new_state[0] < min_floor) continue;
    // Try and move 1 item
    for (int idx = 1; idx < state.size(); idx++) { // Do not try and move the elevator
      // Item not on same floor, cannot move with this item
      if (state[idx] != state[0]) {
        continue;
      }
      new_state[idx] += delta;
      if (valid(new_state, idx)) {
        new_states.push_back(new_state);
      }
      new_state[idx] -= delta;
    }
    // Try and move 2 items
    for (int idx = 1; idx < state.size(); idx++) { // Do not try and move the elevator
      // Item not on same floor, cannot move with this item
      if (state[idx] != state[0]) {
        continue;
      }
      new_state[idx] += delta;
      for (int idx_2 = idx + 1; idx_2 < state.size(); idx_2++) {
        // Item not on same floor, cannot move with this item
        if (state[idx_2] != state[0]) {
          continue;
        }
        new_state[idx_2] += delta;
        if (valid(new_state, idx) && valid(new_state, idx_2)) {
          new_states.push_back(new_state);
        }
        new_state[idx_2] -= delta;
      }
      new_state[idx] -= delta;
    }
  }
  return new_states;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_11_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  // State Elevator, microchips and generators
  std::vector<int> state{1}; // Add elevator state
  std::priority_queue<StateScoreMoves, std::vector<StateScoreMoves>, Comparator> pq;
  
  int floor = 0;
  while(std::getline(file, line)) {
    floor++;
    // std::cout << line << '\n';
    std::vector<std::string_view> split_line;
    std::ranges::copy(line 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string_view(rng);}), 
                            std::back_inserter(split_line));
    bool store_next = false;
    Type type;
    for (const auto word : split_line | std::ranges::views::reverse) {
      if (store_next) {
        state.push_back(floor);
        std::string element = (word.size() > 11 && word[word.size() - 11] == '-') ? std::string{word.substr(0, word.size() - 11)} : std::string{word};
        int id = 0;
        if (const auto it = std::find(std::begin(elements), std::end(elements), element); it == elements.end()) {
          elements.push_back(element);
          id = elements.size() - 1;
        } else {
          id = std::distance(std::begin(elements), it);
        }
        col_titles.emplace_back(type, id);
      }
      if (word.substr(0, 9) == "generator") {
          store_next = true;
          type = Type::GENERATOR;
      } else if (word.substr(0, 9) == "microchip") {
          store_next = true;
          type = Type::MICROCHIP;
      } else {
          store_next = false;
      }
    }
  }
  // for (const auto [idx, ele] : std::ranges::views::enumerate(col_titles)) {
  pairings = std::vector<std::pair<int, int>>(elements.size(), std::pair<int, int>(-1, -1));
  for (int idx = 1; idx < col_titles.size(); idx++) { // Do not add elevator to pairing
    const auto& [type, element_id] = col_titles[idx];
    if (pairings[element_id].first == -1){
      const auto it = std::find_if(
        std::next(std::begin(col_titles), idx + 1), 
        std::end(col_titles), 
        [&element_id](const auto& ele_2) { 
          return element_id == ele_2.second; 
        });
      if (type == Type::GENERATOR) {
        pairings[element_id] = {idx, std::distance(std::begin(col_titles), it)};  
      } else {
        pairings[element_id] = {std::distance(std::begin(col_titles), it), idx};  
      }
    }
  }
  
  const auto score_state = [](const std::vector<int>& state) {
    return std::accumulate(std::begin(state), std::end(state), 0);
  };
  
  StateScoreMoves ssm;
  ssm.state = state;
  ssm.score = score_state(state);
  ssm.moves = 0; 
  pq.push(ssm);
  while(!pq.empty()) {
    const auto current_ssm = pq.top();
    pq.pop();
    if (seen.find(current_ssm.state) != seen.end()) continue;
    if (std::all_of(std::begin(current_ssm.state), std::end(current_ssm.state), [](const auto ele ) {return ele == 4; } )) {
      std::cout << current_ssm.moves << '\n';
      return 0;
    }
    seen.insert(current_ssm.state);
    for (const auto& new_state : get_next_states(current_ssm.state)) {
      if (seen.find(new_state) != seen.end()) continue;
      StateScoreMoves new_ssm;
      new_ssm.score = score_state(new_state);
      new_ssm.state = new_state;
      new_ssm.moves = current_ssm.moves+1; 
      pq.push(new_ssm);
    }
  }
  std::cout << "Not found" << '\n';
  return 0; 
}