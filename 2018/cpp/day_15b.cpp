#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <queue>

#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

enum UnitType {
  ELF,
  GOBLIN
};

struct Point {
  int row, col;
  Point(const int row = 0, const int col = 0) : row(row), col(col){}

  bool operator < (const Point& p) const {
    return row < p.row || (row == p.row && col < p.col);
  }

  bool operator > (const Point& p) const {
    return row > p.row || (row == p.row && col > p.col);
  }

  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }

  bool operator != (const Point& p) const {
    return row != p.row || col != p.col;
  }

  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "("<< p.row << ", " << p.col << ")";
  return os;
}

struct point_hash {
  std::size_t operator () (const Point& p) const {
     // Both work well too, ideally when grid
     // can use number of rows and get id of point
    // return p.row * p.col;
    return (p.row << 4) + p.col;
  }
};

struct Unit {
  UnitType type;
  int hp;
  int ap;

  Unit (const UnitType type, const int hp = 200, const int ap = 3) :
    type(type), hp(hp), ap(ap){}
};

inline void add_keys_to_vec(std::vector<Point> & v, const std::unordered_map<Point, Unit, point_hash>& m) {
  // std::transform(m.begin(), m.end(), std::back_inserter(v), [](const auto& p) { return p.first; })
  // std::copy (bar.begin(),bar.end(),back_inserter(foo));
  for (const auto& i : m) {
    v.push_back(i.first);
  }
};

std::vector<Point> get_attack_positions(const Point& p) {
  return {
    // In reading order
    Point(p.row - 1, p.col),
    Point(p.row, p.col - 1),
    Point(p.row, p.col + 1),
    Point(p.row + 1, p.col)
    // Point(-1,0),
    // Point(0,-1),
    // Point(0,1),
    // Point(1,0)
  };
}

std::tuple<bool, int, std::vector<Point>> dfs (const Point& p, const Point& ap, int max_cost, const std::vector<std::vector<char>>& map) {
  std::queue<std::pair<Point, int>> q;
  std::unordered_map<Point, Point, point_hash> parent;
  std::vector<Point> path;
  q.push({p, 0});
  while(!q.empty()) {
    Point cp = q.front().first;
    const int cc = q.front().second;
    q.pop();
    if (cp == ap) {
      while(cp != p) {
        path.push_back(cp);
        cp = parent[cp];
      }
      path.push_back(cp);
      return {true, cc, path};
    }
    if (cc + 1 <= max_cost) {
      for (const auto& n : get_attack_positions(cp)) {
        if (parent.find(n) == parent.end() && map[n.row][n.col] == '.') {
          q.push(std::make_pair(n, cc+1));
          parent[n] = cp;
        }
      }
    }
  }
  return {false, -1, path};
}

bool compare_paths(const std::vector<Point>& path1, const std::vector<Point>& path2) {
  int index = 0;
  if (path1.size() != path2.size()) {
    return path1.size() < path2.size();
  }
  while(index < path1.size()) {
    if (path1[index] < path2[index]) return true;
    else if (path1[index] > path2[index]) return false;
    index++;
  }
  return true;
}

std::tuple<bool, Point> get_next_move (const Point& p, const std::vector<std::vector<char>>& map, const std::vector<Point>& att_pos) {
  int max_cost = std::numeric_limits<int>::max();
  std::vector<std::pair<int, std::vector<Point>>> results;
  for (const auto& ap : att_pos) {
    const auto [found, cost, path] = dfs(p, ap, max_cost, map);
    if (found) {
      results.emplace_back(cost, path);
      if (cost < max_cost) {
        max_cost = cost;
      }
    }
  }
  if (results.empty()) return {false, Point(-1, -1)};

  const auto it = std::min_element(std::begin(results), std::end(results),
    [](const auto& p1, const auto& p2) {return compare_paths(p1.second, p2.second); });

  return {true,it->second[it->second.size() - 2]};
}

void printGrid(const std::vector<std::vector<char>>& map,
  const std::unordered_map<Point, Unit, point_hash>& units) {
  for (int row = 0; row < map.size(); row++ ) {
    for (int col = 0; col < map[row].size(); col++) {
      std::cout << map[row][col];
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::fstream file(input);
  std::string line;
  std::vector<std::vector<char>> main_map;

  while(std::getline(file, line)) {
    main_map.emplace_back();
    for (const char c : line) {
      main_map.back().emplace_back(c);
    }
  }

  bool elves_win = false;
  int elf_ap = 3;
  while (!elves_win) {
    elf_ap++;
    auto map = main_map;
    std::unordered_map<Point, Unit, point_hash> units;
    for (int row = 0; row < map.size(); row++) {
      for (int col = 0; col < map[row].size(); col++) {
        const auto coord = Point(row, col);
        if (map[row][col] == 'E') {
          units.insert({coord, Unit(UnitType::ELF, 200, elf_ap)});
        } else if (map[row][col] == 'G') {
          units.insert({coord, Unit(UnitType::GOBLIN, 200, 3)});
        }
      }
    }

    std::vector<Point> occupied;

    bool moved_or_attacked = true;
    int step = 0;
    bool round_ends_midway = false;
    bool no_elf_killed = true;
    std::unordered_set<UnitType> n_unit_types;
    while(units.size() > 1 && moved_or_attacked && !round_ends_midway && no_elf_killed) {
      round_ends_midway = false;
      moved_or_attacked = false;
      n_unit_types.clear();

      n_unit_types.insert(std::begin(units)->second.type);
      for (const auto& unit : units) {
        if (const auto it = n_unit_types.insert(unit.second.type); it.second)  {
          break;
        }
      }
      if (n_unit_types.size() == 1) {
        break;
      }
      occupied.clear();
      add_keys_to_vec(occupied, units);
      std:sort(std::begin(occupied), std::end(occupied));
      step++;
      // std::cout << step << '\n';
      for (int pos_i = 0; pos_i < occupied.size(); pos_i++) {

        n_unit_types.clear();
        n_unit_types.insert(std::begin(units)->second.type);
        for (const auto& unit : units) {
          if (const auto it = n_unit_types.insert(unit.second.type); it.second)  {
            break;
          }
        }
        if (n_unit_types.size() == 1) {
          round_ends_midway = true;
          break;
        }

        if (units.find(occupied[pos_i]) == units.end()) continue; //unit has been killed
        Point position = occupied[pos_i];
        std::vector<Point> att_pos;

        // Move
        bool need_to_move = true;
        for(const auto& n : get_attack_positions(position)) {
          if (units.find(n) != units.end() &&
              units.find(occupied[pos_i]) != units.end() &&
              units.find(n)->second.type != units.find(occupied[pos_i])->second.type) {
            need_to_move = false;
            break;
          }
        }

        if (need_to_move) {
          const auto unit = units.find(occupied[pos_i])->second;
          units.erase(occupied[pos_i]);
          for (const auto& p : units) {
            if (p.second.type != unit.type) {
              for (const auto& ap : get_attack_positions(p.first)) {
                att_pos.push_back(ap);
              }
            }
          }
          const auto [found, new_position] = get_next_move(occupied[pos_i], map, att_pos);
          if (found) {
            map[position.row][position.col] = '.';
            position = new_position;
            moved_or_attacked = true;
            if (unit.type == UnitType::ELF) {
              map[position.row][position.col] = 'E';
            } else if (unit.type == UnitType::GOBLIN) {
              map[position.row][position.col] = 'G';
            }
          }
          units.insert({position, unit});
        }

        // Attack
        std::vector<Point> opponent_positions;
        for(const auto& n : get_attack_positions(position)) {
          if (units.find(n) != units.end() &&
              units.find(position) != units.end() &&
              units.find(n)->second.type != units.find(position)->second.type) {
            opponent_positions.push_back(n);
          }
        }
        if (opponent_positions.size() > 0) {
          auto p = *std::min_element(std::begin(opponent_positions), std::end(opponent_positions),
                   [&units](const Point& p1, const Point& p2) { return units.find(p1)->second.hp < units.find(p2)->second.hp; } );
          units.find(p)->second.hp -= units.find(position)->second.ap;
          moved_or_attacked = true;

          if (units.find(p)->second.hp <= 0) {
            if (units.find(p)->second.type == UnitType::ELF) {
              no_elf_killed = false;
              break;
            }
            units.erase(p);
            map[p.row][p.col] = '.';
          }
        }
      }
      // printGrid(map, units);
    }
    if (round_ends_midway) step--;
    if (*std::begin(n_unit_types) == UnitType::ELF && no_elf_killed) {
      std::cout << "Elf ap: " << elf_ap << '\n';
      auto sum_hp = std::accumulate(std::begin(units), std::end(units), 0,
        [](int total_hp, const auto& p) { return total_hp + p.second.hp; });
      std::cout << sum_hp << ' ' << step << ' ' << sum_hp * step << '\n';
      break;
    }
  }
  return 0;
}
