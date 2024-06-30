#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

int sv_to_int (const std::string_view sv) {
  int n = 0;
  for (const auto c : sv) {
    n = n * 10 + (c - '0');
  }
  return n;
}

enum class to_type {
  BOT, OUTPUT
};

struct Bot {
  int id;
  std::pair<to_type, int> low;
  std::pair<to_type, int> high;
  std::array<std::pair<bool, int>, 2> values = {{
    std::pair<bool, int>{false, -1},
    std::pair<bool, int>{false, -1}
  }};

  bool has_two_values() const {
    return values[0].first && values[1].first;
  }
  
  void add_value(const int value) {
    if (!values[0].first) {
      values[0].first = true;
      values[0].second = value;
    }
    else {
      values[1].first = true;
      values[1].second = value;
    }
    if (has_two_values() && values[0].second > values[1].second) {
      std::swap(values[0].second, values[1].second);
    }
  }
  
  void clear() {
    values[0].first = false;
    values[1].first = false;
  }

  friend std::ostream& operator << (std::ostream & os, const Bot& b); 

};

std::ostream& operator << (std::ostream & os, const Bot& bot) {
  os << "----- Bot -----" << '\n'
     << "id: " << bot.id << '\n'
     << "low: " << static_cast<int>(bot.low.first) << ',' << bot.low.second << '\n'
     << "high: " << static_cast<int>(bot.high.first) << ',' << bot.high.second << '\n'
     << "values: " << "(" << bot.values[0].first << ',' << bot.values[0].second << "),("<< bot.values[1].first << ',' << bot.values[1].second << ")" << '\n';
  return os;     
}

struct hasher {
  std::size_t operator () (const Bot& bot) const {
    return bot.id;
  }
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_10_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  std::unordered_map<int, Bot> bots;
  std::unordered_map<int, int> outputs;
  while(std::getline(file, line)) {
    std::vector<std::string_view> split_line;
    std::ranges::copy(line 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string_view(rng);}), 
                            std::back_inserter(split_line));
    if (split_line.size() == 6) {
      const int id = sv_to_int(split_line[5]); 
      if (bots.find(id) == bots.end()) {
        Bot b;
        b.id = id;
        bots[id] = b;
      }  
      bots[id].add_value(sv_to_int(split_line[1]));
    } else {
      const int id = sv_to_int(split_line[1]); 
      if (bots.find(id) == bots.end()) {
        Bot b;
        b.id = id;
        bots[id] = b;
      }
      bots[id].low.first = (split_line[5] == "output") ? to_type::OUTPUT : to_type::BOT; ;
      bots[id].low.second = sv_to_int(split_line[6]);
      bots[id].high.first = (split_line[10] == "output") ? to_type::OUTPUT : to_type::BOT; ;
      bots[id].high.second = sv_to_int(split_line[11]);
    }
  }
  std::stack<int> bot_ids;
  for (const auto& [id, bot] : bots) {
    if (bot.has_two_values()) {
      bot_ids.push(id);
    }
  }
  
  while(!bot_ids.empty()) {
    const auto id = bot_ids.top();
    bot_ids.pop();
    auto& bot = bots[id];
    if (!bot.has_two_values()) continue;
    if (bot.low.first == to_type::BOT) {
      bots[bot.low.second].add_value(bot.values[0].second);
    } else {
      outputs[bot.low.second] = bot.values[0].second;
    }
    if (bot.high.first == to_type::BOT) {
      bots[bot.high.second].add_value(bot.values[1].second);
    } else {
      outputs[bot.high.second] = bot.values[1].second;
    }
    bot.clear();
    if (bots[bot.low.second].has_two_values()) {
      bot_ids.push(bot.low.second);
    }
    if (bots[bot.high.second].has_two_values()) {
      bot_ids.push(bot.high.second);
    }
  }
  std::cout << outputs[0] * outputs[1] * outputs[2] << '\n';
  return 0;
}