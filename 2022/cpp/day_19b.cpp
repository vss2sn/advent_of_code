#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// TODO: redo with std::array instead of custom types and optimize.
// Wait only if the next action is_helpful() and !is_possible(), not just the latter.
// Without the is_helpful() in the condition,
// only the cheapest bots will be built until the maximum required is hit, then the secon cheapest, and so on.

struct Resource {
  int ore = 0;
  int clay = 0;
  int obsidian = 0;
  int geodes = 0;

  Resource operator - (const Resource& r) const {
    auto ans = Resource();
    ans.ore = ore - r.ore;
    ans.clay = clay - r.clay;
    ans.obsidian = obsidian - r.obsidian;
    ans.geodes = geodes - r.geodes;
    return ans;
  }

  bool operator == (const Resource& r) const {
    return ore == r.ore && clay == r.clay && obsidian == r.obsidian && geodes == r.geodes;
  }

  bool operator < (const Resource& r) const {
    return ore < r.ore && clay < r.clay && obsidian < r.obsidian && geodes < r.geodes;
  }

  bool operator >= (const Resource& r) const {
    return ore >= r.ore && clay >= r.clay && obsidian >= r.obsidian && geodes >= r.geodes;
  }

  friend std::ostream& operator<<(std::ostream& os, const Resource& r);
};

std::ostream& operator<<(std::ostream& os, const Resource& r) {
  os << '('  <<
  r.ore << ',' <<
  r.clay << ',' <<
  r.obsidian << ',' <<
  r.geodes << ')' <<
  '\n';
  return os;
}

struct Blueprint {
  int id = 0;
  Resource ore_bot_cost;
  Resource clay_bot_cost;
  Resource obsidian_bot_cost;
  Resource geode_bot_cost;
  Resource max_cost_of_each;
  int max_geodes = 0;

  void update_max_cost_of_each() {
    const auto all_costs = std::vector<Resource> {
      ore_bot_cost,
      clay_bot_cost,
      obsidian_bot_cost,
      geode_bot_cost
    };
    max_cost_of_each.ore = std::max_element(std::begin(all_costs), std::end(all_costs), [](const auto& r1, const auto& r2) {return r1.ore < r2.ore;} )->ore;
    max_cost_of_each.clay = std::max_element(std::begin(all_costs), std::end(all_costs), [](const auto& r1, const auto& r2) {return r1.clay < r2.clay;} )->clay;
    max_cost_of_each.obsidian = std::max_element(std::begin(all_costs), std::end(all_costs), [](const auto& r1, const auto& r2) {return r1.obsidian < r2.obsidian;} )->obsidian;
    max_cost_of_each.geodes = std::max_element(std::begin(all_costs), std::end(all_costs), [](const auto& r1, const auto& r2) {return r1.geodes < r2.geodes;} )->geodes;
  }
  friend std::ostream& operator<<(std::ostream& os, const Blueprint& b);
};

std::ostream& operator<<(std::ostream& os, const Blueprint& b) {
  os <<
  b.id << '\n' << ' ' <<
  b.ore_bot_cost << ' ' <<
  b.clay_bot_cost << ' ' <<
  b.obsidian_bot_cost << ' ' <<
  b.geode_bot_cost << ' ' <<
  b.max_cost_of_each <<
  b.max_geodes << '\n';
  return os;
}

struct State {
  int time = 0;
  int ore = 0;
  int ore_bots = 0;
  int clay_bots = 0;
  int obsidian_bots = 0;
  int geode_bots = 0;
  Resource resources;

  friend std::ostream& operator<<(std::ostream& os, const State& s);
};

std::ostream& operator<<(std::ostream& os, const State& s) {
  os <<
  s.time << ' ' <<
  s.ore << ' ' << '(' <<
  s.ore_bots << ',' <<
  s.clay_bots << ',' <<
  s.obsidian_bots << ',' <<
  s.geode_bots << ')' << ' ' <<
  s.resources;
  return os;
}

inline int calc_theoretical_max (const State& s) {
  return s.resources.geodes + (s.time + 2 * s.geode_bots) * s.time / 2;
}

enum class Action {
  WAIT,
  ORE,
  CLAY,
  OBSIDIAN,
  GEODE
};

inline bool is_action_helpful (const State& state, const Blueprint& blueprint, const Action action) {
  if (action == Action::WAIT) {
    return !(blueprint.max_cost_of_each < state.resources);
  } else if (action == Action::ORE) {
    return state.ore_bots < blueprint.max_cost_of_each.ore;
  } else if (action == Action::CLAY) {
    return state.clay_bots < blueprint.max_cost_of_each.clay;
  } else if (action == Action::OBSIDIAN) {
    return state.obsidian_bots < blueprint.max_cost_of_each.obsidian;
  }
  return true;
}


inline bool is_action_possible (const State& state, const Blueprint& blueprint, const Action action) {
  if (action == Action::ORE) {
    return state.resources >= blueprint.ore_bot_cost;
  } else if (action == Action::CLAY) {
    return state.resources >= blueprint.clay_bot_cost;
  } else if (action == Action::OBSIDIAN) {
    return state.resources >= blueprint.obsidian_bot_cost;
  }
  return state.resources >= blueprint.geode_bot_cost;
}

inline void update_state(State& state) {
  state.time-=1 ;
  state.resources.ore += state.ore_bots;
  state.resources.clay += state.clay_bots;
  state.resources.obsidian += state.obsidian_bots;
  state.resources.geodes += state.geode_bots;
}

State perform_action (State state, const Blueprint& blueprint, const Action action) {
if (action == Action::ORE) {
    state.resources = state.resources - blueprint.ore_bot_cost;
    update_state(state);
    state.ore_bots++;
  } else if (action == Action::CLAY) {
    state.resources = state.resources - blueprint.clay_bot_cost;
    update_state(state);
    state.clay_bots++;
  } else if (action == Action::OBSIDIAN) {
    state.resources = state.resources - blueprint.obsidian_bot_cost;
    update_state(state);
    state.obsidian_bots++;
  } else {
    state.resources = state.resources - blueprint.geode_bot_cost;
    update_state(state);
    state.geode_bots++;
  }
  return state;
}

void dfs (State state, const Blueprint& blueprint, int& max_geodes, const std::array<Action, 4>& actions, const Action action){ //}, std::vector<std::pair<State, Action>>& history) {
  if (!is_action_helpful(state, blueprint, action)) return;
  if(calc_theoretical_max(state) <= max_geodes) return;
  while (state.time > 0) {
    if (is_action_possible(state, blueprint, action)) {
      const auto new_state = perform_action(state, blueprint, action);
      for (const auto new_action : actions) {
        dfs(new_state, blueprint, max_geodes, actions, new_action);
      }
      return;
    } else {
      // Wait iff action is helpful, but not currently possible
      update_state(state);
    }
  }
  max_geodes = std::max(max_geodes, state.resources.geodes);
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Blueprint> blueprints;
  const std::regex pattern(R"(Blueprint ([0-9]+): Each ore robot costs ([0-9]+) ore. Each clay robot costs ([0-9]+) ore. Each obsidian robot costs ([0-9]+) ore and ([0-9]+) clay. Each geode robot costs ([0-9]+) ore and ([0-9]+) obsidian.)");
  while (std::getline(file, line)) {
    std::smatch match;
    std::regex_match(line, match, pattern);
    auto b = Blueprint();
    b.id = std::stoi(match[1]);
    auto cost = Resource();
    cost.ore = std::stoi(match[2]);
    b.ore_bot_cost = cost;
    cost.ore = std::stoi(match[3]);
    b.clay_bot_cost = cost;
    cost.ore = std::stoi(match[4]);
    cost.clay = std::stoi(match[5]);
    b.obsidian_bot_cost = cost;
    cost.ore = std::stoi(match[6]);
    cost.clay = 0;
    cost.obsidian = std::stoi(match[7]);
    b.geode_bot_cost = cost;
    b.update_max_cost_of_each();
    blueprints.push_back(b);
  }

  constexpr std::array<Action, 4> actions { Action::ORE, Action::CLAY, Action::OBSIDIAN, Action::GEODE };
  // std::reverse(std::begin(actions), std::end(actions));
  auto init_state = State();
  init_state.time = 32;
  init_state.ore_bots = 1;
  int ans = 1;
  for (int i = 0; i < 3; i++) {
    int max_geodes = -1;
    for (const auto action : actions) {
      dfs(init_state, blueprints[i], max_geodes, actions, action);
    }
    blueprints[i].max_geodes = max_geodes;
    ans *= blueprints[i].max_geodes;
  }
  std::cout << ans << '\n';
  return 0;
}
