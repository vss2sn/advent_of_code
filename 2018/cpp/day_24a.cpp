#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <numeric>

// A lot of this code is to print debg output exactly like the example
constexpr bool debug = false;

struct Group {
  int id;
  int n_units;
  int hp;
  int attack_damage;
  int initiative;
  int effective_power;
  std::string attack_type;
  std::vector<std::string> weaknesses;
  std::vector<std::string> immunities;

  void update_effective_power() {
    effective_power = n_units * attack_damage;
  }
};

std::ostream& operator << (std::ostream& os, const Group& g) {
  os << "----- Group -----" << '\n' <<
  "id:" << g.id << '\n' <<
  "n_units: " << g.n_units << '\n' <<
  "hp: " << g.hp << '\n' <<
  "attack_damage: " << g.attack_damage << '\n' <<
  "attack_type: " << g.attack_type << '\n' <<
  "initiative: " << g.initiative << '\n' <<
  "effective_power: " << g.effective_power << '\n';
  os << "weaknesses: ";
  if (g.weaknesses.size() > 0) {
    os << '|';
    for (const auto& w : g.weaknesses) {
      os << w << "|";
    }
  }
  os << '\n';
  os << "immunities: ";
  if (g.immunities.size() > 0) {
    os << '|';
    for (const auto& i : g.immunities) {
      os << i << "|";
    }
  }
  os << '\n';
  os << "----- -----"<< '\n';
  return os;
}

struct Army {
  std::string name;
  int id;
  std::vector<Group> groups;
  std::unordered_map<int, int> g_tg_map; // group to target group map

  void reset_tgi() {
    g_tg_map.clear();
    for (auto& g : groups) {
      g_tg_map[g.id] = -1;
    }
  }

  int get_units() {
    return std::accumulate(std::begin(groups), std::end(groups), 0,
      [](const auto& sum, const auto& g) { return sum + g.n_units; }
    );
  }

  bool is_alive() {
    return std::find_if(
      std::begin(groups),
      std::end(groups),
      [](const auto& g) {return g.n_units > 0;}
    ) != std::end(groups);
  }

  void update_effective_power() {
    for (auto& g : groups) {
      g.update_effective_power();
    }
  }
};

std::ostream& operator << (std::ostream& os, const Army& a) {
  os << "---------- Army ----------"<< '\n';
  os << "name: "<< a.name << '\n';
  os << "Groups:"<< '\n';
  for (const auto& group : a.groups) {
    os << group.n_units << '\n';
  }
  os << "---------- ----------"<< '\n';
  return os;
}

std::vector<std::string> parse_attributes(const std::string& attribute_str, const int offset=0) {
  std::vector<std::string> attributes;
  const std::string delimiter = ", ";
  size_t start = 0;
  size_t end = attribute_str.find(delimiter, start);
  while(end != std::string::npos) {
    attributes.push_back(attribute_str.substr(start, end - start));
    start = end + delimiter.size();
    end = attribute_str.find(delimiter, start);
  }
  attributes.push_back(attribute_str.substr(start, attribute_str.size() - start));
  return attributes;
}

void parse_attributes (std::string attribute_str, Group& group) {
  if (attribute_str.size() == 0) return;
  attribute_str = attribute_str.substr(1, attribute_str.size() - 3);
  const auto split_i = attribute_str.find_first_of(';');
  if (attribute_str[0] == 'w') {
    group.weaknesses = parse_attributes(attribute_str.substr(8, split_i - 8));
    if (split_i != std::string::npos) {
      group.immunities = parse_attributes(attribute_str.substr(split_i+2+10, attribute_str.size() - split_i - 2 - 10));
    }
  } else {
    group.immunities = parse_attributes(attribute_str.substr(10, split_i - 10));
    if (split_i != std::string::npos) {
      group.weaknesses = parse_attributes(attribute_str.substr(split_i+2+8, attribute_str.size() - split_i - 2 - 8));
    }
  }
}


int select_target(Group& g, const std::vector<Group>& t_groups, std::unordered_set<int>& selected_targets, const std::string& debug_str) {
  int max_damage = 0;
  int tg_i = -1;
  if (g.n_units == 0) return -1;
  for (int i = 0; i < t_groups.size(); i++) {
    auto& tg = t_groups[i];
    if (tg.n_units == 0) continue;
    if (selected_targets.find(tg.id) != selected_targets.end()) continue;
    auto damage = g.n_units * g.attack_damage;
    if (std::find(std::begin(tg.immunities), std::end(tg.immunities), g.attack_type) != tg.immunities.end()) {
      damage = 0;
      continue;
    }
    if (std::find(std::begin(tg.weaknesses), std::end(tg.weaknesses), g.attack_type) != tg.weaknesses.end()) {
      damage *= 2;
    }
    if (damage == 0) continue;
    if constexpr (debug) std::cout << debug_str << ' ' <<  g.id << " would deal defending group " << tg.id << ' ' << damage << " damage " << '\n';
    if (damage > max_damage) {
      tg_i = i;
      max_damage = damage;
    } else if ((damage == max_damage) && (tg_i == -1 || (t_groups[tg_i].effective_power < tg.effective_power))) {
      tg_i = i;
    } else if ((damage == max_damage) && (t_groups[tg_i].effective_power == tg.effective_power) && (t_groups[tg_i].initiative < tg.initiative)) {
      tg_i = i;
    }
  }
  if (tg_i != -1) {
    selected_targets.insert(t_groups[tg_i].id);
    return t_groups[tg_i].id;
  }
  return -1;
}

void target_selection(Army& a1, Army& a2) {
  auto selected_targets = std::unordered_set<int>();
  for (auto & g1 : a1.groups) {
    const auto tg_i = select_target(g1, a2.groups, selected_targets, a1.name);
    if (tg_i != -1) {
      a1.g_tg_map[g1.id] = tg_i;
    }
  }
  selected_targets.clear();
  for (auto& g2 :  a2.groups) {
    const auto tg_i = select_target(g2, a1.groups, selected_targets, a2.name);
    if (tg_i != -1) {
      a2.g_tg_map[g2.id] = tg_i;
    }
  }
}

struct InitStruct {
  int initiative, a_id, g_id;
};

void attack(std::vector<Army>& armies, std::vector<InitStruct>& init_order) {
  for (auto& io : init_order) {
    auto& a1 = armies[io.a_id];
    auto& a2 = armies[armies.size() - io.a_id - 1];
    auto& g1 = *std::find_if(
          std::begin(a1.groups),
          std::end(a1.groups),
          [&io](const auto& g1) {
            return g1.id == io.g_id;
          }
        );
    if (g1.n_units == 0) continue;
    if (a1.g_tg_map[g1.id] == -1) continue;
    auto& g2 = *std::find_if(
      std::begin(a2.groups),
      std::end(a2.groups),
      [&a1, &g1](const auto& g2) {
        return g2.id == a1.g_tg_map[g1.id];
      }
    );
    if (g2.n_units == 0) continue;
    int damage = g1.n_units * g1.attack_damage;
    if (std::find(std::begin(g2.weaknesses), std::end(g2.weaknesses), g1.attack_type) != g2.weaknesses.end()) {
      damage *= 2;
    }
    g2.n_units = std::max(static_cast<int>(0), g2.n_units - damage/g2.hp);
    if constexpr (debug) std::cout << a1.name << " group " << g1.id + 1 << " attacks defending group " << g2.id + 1 << ", killing " << damage/g2.hp << " units" << '\n';
  }
}

void print_army_status(const std::vector<Army>& armies, const int immune_system_army_index, const int infection_army_index) {
  const auto& immune_system_army = armies[immune_system_army_index];
  std::cout << immune_system_army.name << ":\n";
  for (int i = 0; i < immune_system_army.groups.size(); i++) {
    const auto g = *std::find_if(std::begin(immune_system_army.groups), std::end(immune_system_army.groups),
      [&i](const auto& g) {return g.id == i;});
    std::cout << "Group " << i + 1 << " contains " << g.n_units << " units" << '\n';
  }

  const auto& infection_army = armies[infection_army_index];
  std::cout << infection_army.name << ":\n";
  for (int i = 0; i < infection_army.groups.size(); i++) {
    const auto g = *std::find_if(std::begin(infection_army.groups), std::end(infection_army.groups),
      [&i](const auto& g) {return g.id == i;});
    std::cout << "Group " << i + 1 << " contains " << g.n_units << " units" << '\n';
  }
  std::cout << '\n';
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::vector<Army> armies;
  std::ifstream file(input);
  std::vector<InitStruct> init_order;
  int g_id = 0;
  int infection_army_index = -1;
  int immune_system_army_index = -1;
  const std::regex pattern(R"(([0-9]+) units each with ([0-9]+) hit points ([a-z,\(,\),\;,\,,\ ,]+)?with an attack that does ([0-9]+) ([a-z]+) damage at initiative ([0-9]+))");
  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line.size() == 0) continue;
    if (const auto i = line.find(':'); i != std::string::npos) {
      auto army = Army();
      g_id = 0;
      army.name = line.substr(0, i);
      if (army.name == "Immune System") {
        immune_system_army_index = armies.size();
      }
      if (army.name == "Infection") {
        infection_army_index = armies.size();
      }
      army.id = armies.size();
      armies.push_back(army);
      continue;
    }
    std::smatch match;
    std::regex_search(line, match, pattern);
    auto group = Group();
    group.id = g_id;
    group.n_units = std::stoi(match[1]);
    group.hp = std::stoi(match[2]);
    parse_attributes(match[3], group);
    group.attack_damage = std::stoi(match[4]);
    group.effective_power = group.n_units * group.attack_damage;
    group.attack_type = match[5];
    group.initiative = std::stoi(match[6]);
    armies.back().groups.push_back(group);

    auto init_struct = InitStruct();
    init_struct.initiative = group.initiative;
    init_struct.g_id = g_id;
    init_struct.a_id = armies.back().id;
    init_order.push_back(init_struct);
    g_id++;
  }
  std::sort(std::begin(init_order), std::end(init_order), [](const auto& io1, const auto& io2 ) {return io1.initiative > io2.initiative; });
  for (const auto& io : init_order)  {
    auto& a = armies[io.a_id];
    auto& g = *std::find_if(
      std::begin(a.groups),
      std::end(a.groups),
      [&io](const auto& g) {
        return g.id == io.g_id;
      }
    );
  }
  while (armies[0].is_alive() && armies[1].is_alive()) {
    if constexpr(debug) print_army_status(armies, immune_system_army_index, infection_army_index);
    for (auto& army : armies) {
      army.reset_tgi();
      army.update_effective_power();
      const auto sort_attacking_order = [](const auto& g1, const auto& g2) {
        return (g1.effective_power > g2.effective_power) ||
               ((g1.effective_power == g2.effective_power) && (g1.initiative > g2.initiative));
      };
      std::sort(std::begin(army.groups), std::end(army.groups), [&sort_attacking_order](const auto& g1, const auto& g2) { return sort_attacking_order(g1, g2); } );
    }
    target_selection(armies[infection_army_index], armies[immune_system_army_index]);
    if constexpr(debug) std::cout << '\n';
    attack(armies, init_order);
    if constexpr(debug) std::cout << '\n';
  }

  if (armies[0].is_alive()) {
    const auto ans = armies[0].get_units();
    std::cout << ans << '\n';
    return ans;
  }
  const auto ans = armies[1].get_units();
  std::cout << ans << '\n';
  return ans;
}
