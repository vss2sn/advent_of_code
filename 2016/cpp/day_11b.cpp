#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>    
#include <numeric>    
#include <iomanip> 

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
std::unordered_map<int, std::pair<int, int>> pairings; // Map of element id to where colun index of (generator, microchip)
std::unordered_set<std::vector<int>, hash_state> seen;
std::vector<int> change_floor{-1,1}; // Moves 
constexpr int max_floor = 4;
constexpr int min_floor = 1;

bool valid(const std::vector<int>& state, int col) {
  // std::cout << "Attempting to validate" << '\n';
  const auto& [type, id] = col_titles[col];
  const auto& [gen_col, chip_col] = pairings[id];
  const auto gen_floor = state[gen_col];
  const auto chip_floor = state[chip_col];
  // std::cout << static_cast<int>(type)<< ' ' << id << ' ' << gen_col <<  ' ' << chip_col <<  ' ' << gen_floor <<  ' ' << chip_floor << '\n';
  // Both microchip and generator of the moved item are now at the same level
  const bool chip_safe = (gen_floor == chip_floor) ? true : false;
  // std::cout << "Chip safe: " << chip_safe << '\n';
  // Check whether an unconnected chip exists on gen_floor and a generator exists on chip_floor
  for (const auto& [ele_id, ele_cols] : pairings) {
    const auto ele_gen_floor = state[ele_cols.first];
    const auto ele_chip_floor = state[ele_cols.second];
    const auto ele_chip_safe = (ele_gen_floor == ele_chip_floor);
    // std::cout << "ele chip " << ele_id << " safe: " << chip_safe << '\n';
    if (!chip_safe && ele_gen_floor == chip_floor) {
      // std::cout << "No " << '\n';
      return false; // If true then ele_gen will fry the chip
    }
    if (!ele_chip_safe && ele_chip_floor == gen_floor) {
      // std::cout << "No " << '\n';
      return false; // If true then gen will fry the ele_chip
    }
  }
  // std::cout << "Valid " << '\n';
  return true;
}

constexpr int max_items_movable = 2;
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
      // if (seen.find(new_state) != seen.end()) continue;
      // std::cout << "Possible new state" << '\n';
      // for (const auto& ele : new_state) {
      //     std::cout << ele << ' ';
      //   }
      //   std::cout << '\n';
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
        // std::cout << "Possible new state" << '\n';
        // for (const auto& ele : new_state) {
        //   std::cout << ele << ' ';
        // }
        // std::cout << '\n';
        // if (seen.find(new_state) != seen.end()) continue;
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
  
  int floor = 0;
  while(std::getline(file, line)) {
    floor++;
    std::cout << line << '\n';
    std::vector<std::string_view> split_line;
    std::ranges::copy(line 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string_view(rng);}), 
                            std::back_inserter(split_line));
    bool store_next = false;
    Type type;
    for (const auto ele : split_line | std::ranges::views::reverse) {
      if (store_next) {
        std::cout << "----- storing next ------" << '\n';
       std::cout << ele << '\n';
       std::cout << floor << '\n';
        state.push_back(floor);
        std::cout << state.size() << ' ' << state.back() << '\n';
        // std::cout << ele.substr(ele.size() - 11, 11) << '\n';
        std::string element = (ele.size() > 11 && ele[ele.size() - 11] == '-') ? std::string{ele.substr(0, ele.size() - 11)} : std::string{ele};
        int id = 0;
        if (const auto it = std::find(std::begin(elements), std::end(elements), element); it == elements.end()) {
          elements.push_back(element);
          id = elements.size() - 1;
        } else {
          id = std::distance(std::begin(elements), it);
        }
        col_titles.emplace_back(type, id);
      }
      if (ele.substr(0, 9) == "generator") {
          store_next = true;
          type = Type::GENERATOR;
      } else if (ele.substr(0, 9) == "microchip") {
          store_next = true;
          type = Type::MICROCHIP;
      } else {
          store_next = false;
      }
    }
  }
  // for (const auto [idx, ele] : std::ranges::views::enumerate(col_titles)) {
  for (int idx = 1; idx < col_titles.size(); idx++) { // Do not add elevator to pairing
    const auto& ele = col_titles[idx];
    std::cout << idx << ": " << state[idx] << ' ' << '('<< static_cast<int>(ele.first) << ',' << ele.second << ')' << '\n';
    if (pairings.find(ele.second) == pairings.end()){
      const auto it = std::find_if(
        std::next(std::begin(col_titles), idx + 1), 
        std::end(col_titles), 
        [&ele](const auto& ele_2) { 
          return ele.second == ele_2.second; 
        });
      if (ele.first == Type::GENERATOR) {
        pairings[ele.second] = {idx, std::distance(std::begin(col_titles), it)};  
      } else {
        pairings[ele.second] = {std::distance(std::begin(col_titles), it), idx};  
      }
      std::cout << "Pairing " << ele.second << ": (" << pairings[ele.second].first << "," << pairings[ele.second].second << ")" << '\n';
    }
  }
  // max_floor = floor;
  // std::cout << "Max floor " << max_floor << '\n';
  std::priority_queue<StateScoreMoves, std::vector<StateScoreMoves>, Comparator> pq;
  
  const auto score_state = [](const std::vector<int>& state) {
    return std::accumulate(std::begin(state), std::end(state), 0);
  };
  
  StateScoreMoves ssm;
  ssm.state = state;
  ssm.score = score_state(state);
  ssm.moves = 0; 
  pq.push(ssm);
  // int count = 0;
  while(!pq.empty()) {
    // count++;
    // if (count == 10) exit(0);
    // if (seen.size() > 4194304) {
    //   std::cout << "Seen more states than possibel" << '\n';
    // }
    const auto current_ssm = pq.top();
    pq.pop();
    if (seen.find(current_ssm.state) != seen.end()) continue;

    // std::cout << std::setw(8) << elemets[col_titles[i]][0] << ' ';
    // for (int i = 1; i < current_state.size(); i++) {
    //   char temp;
    //   if (static_cast<int>(elemets[col_titles[i]].first) == 0) temp = 'G';
    //   if (static_cast<int>(elemets[col_titles[i]].first) == 0) temp = 'M';
    //   std::cout << std::setw(8) << elemets[col_titles[i]][0] << ' ';
    // }
    // std::cout << " | ";
    // std::cout << "Score: " << current_ssm.score << '\n';
    // std::cout << "Current state: ";
    // for (const auto& ele : current_ssm.state) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << " | ";
    // std::cout << "Score: " << current_ssm.score << '\n';
    // std::cout << "Max score :" <<  state.size() * 4 << '\n';
    if (std::all_of(std::begin(current_ssm.state), std::end(current_ssm.state), [](const auto ele ) {return ele == 4; } )) {
    // if (current_ssm.score == (state.size() * 4)) {
      std::cout << "Found" << '\n';
      for (const auto ele : current_ssm.state) {
        std::cout << ele;
      }
      std::cout << '\n';
      std::cout << current_ssm.moves << '\n';
      return 0;
    }
    seen.insert(current_ssm.state);
    for (const auto& new_state : get_next_states(current_ssm.state)) {
      if (seen.find(new_state) != seen.end()) continue;
      // std::cout << "Adding: ";
      // for (const auto& ele : new_state) {
      //   std::cout << ele << ' ';
      // }
      // std::cout << '\n';
      StateScoreMoves new_ssm;
      new_ssm.score = score_state(new_state);
      new_ssm.state = new_state;
      new_ssm.moves = current_ssm.moves+1; 
      pq.push(new_ssm);
      // std::cout << "Added: ";
      // for (const auto& ele : new_ssm.state) {
      //   std::cout << ele << ' ';
      // }
      // std::cout << '\n';
    }
    // exit(0);
  }
  std::cout << "Not found" << '\n';
  return 0; 
}