#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

enum class Tools {
  TORCH,
  CLIMBING_GEAR,
  NEITHER
};

enum class RegionType {
  ROCKY,
  WET,
  NARROW
};

inline constexpr std::array<Tools, 2> get_allowed_tools (const RegionType& rt) {
  if (rt == RegionType::ROCKY) {
    return {Tools::CLIMBING_GEAR, Tools::TORCH};
  } else if (rt == RegionType::WET) {
    return {Tools::CLIMBING_GEAR, Tools::NEITHER};
  } else if (rt == RegionType::NARROW){
    return {Tools::NEITHER, Tools::TORCH};
  } else {
    std::cout << "This should not happen" << '\n';
    exit(0);
  }
}

struct MapPoint {
  int row;
  int col;
  long geo_index;
  int erosion;
  RegionType region_type;

  MapPoint(const int row, const int col, const long geo_index=0, const int erosion=0, const RegionType region_type=RegionType::ROCKY) :
  row(row), col(col), geo_index(geo_index), erosion(erosion), region_type(region_type) {}

  bool operator == (const MapPoint& p) const {
    return p.row == row && p.col == col;
  }
};

struct QueuePoint {
  int row;
  int col;
  int cost;
  Tools tool;

  QueuePoint(const int row, const int col, const int cost, const Tools tool) :
  row(row), col(col), cost(cost), tool(tool) {}

  bool operator == (const QueuePoint& p) const {
    return p.row == row && p.col == col && p.tool == tool;
  }

  bool operator < (const QueuePoint& p) const {
    return cost > p.cost;
  }
};

struct mp_hasher {
  int operator() (const MapPoint& mp) const noexcept {
    return mp.row * mp.row * mp.col;
  }
};

struct qp_hasher {
  int operator() (const QueuePoint& qp) const noexcept {
    return qp.row * qp.row * qp.col;
  }
};

using map2d = std::unordered_set<MapPoint, mp_hasher>;

RegionType get_type_from_erosion(const int erosion) {
  if (const int type = erosion % 3; type == 0) {
    return RegionType::ROCKY;
  } else if (type == 1) {
    return RegionType::WET;
  } else {
    return RegionType::NARROW;
  }
}

MapPoint add_point_to_map(map2d& map, const int row, const int col, const int depth) {
  auto p = MapPoint(row, col);
  if (const auto it = map.find(p); it != map.end()) return *it;

  if (p.row > 0 && p.col > 0) {
    const auto point_to_left = add_point_to_map(map, row, col-1, depth);
    const auto point_above = add_point_to_map(map, row-1, col, depth);
    p.geo_index = point_above.erosion * point_to_left.erosion;
    p.erosion = (p.geo_index + depth) % 20183;
    p.region_type = get_type_from_erosion(p.erosion);
  } else if (p.row == 0) {
    p.geo_index = p.col * 16807;
    p.erosion = (p.geo_index + depth) % 20183;
    p.region_type = get_type_from_erosion(p.erosion);
  } else if (p.col == 0) {
    p.geo_index = p.row * 48271;
    p.erosion = (p.geo_index + depth) % 20183;
    p.region_type = get_type_from_erosion(p.erosion);
  } else {
    std::cout << "This should not occur" << '\n';
    std::cout << p.row << ' ' << p.col << '\n';
    exit(0);
  }
  map.insert(p);
  return p;
}


int find_path(map2d& map, const MapPoint& start, const MapPoint& target, const int depth) {
  std::priority_queue<QueuePoint> pq;
  std::unordered_set<QueuePoint, qp_hasher> seen;
  pq.emplace(start.row, start.col, 0, Tools::TORCH);
  while (!pq.empty()) {
    const auto current_qp = pq.top();
    const auto current_mp = add_point_to_map(map, current_qp.row, current_qp.col, depth);
    pq.pop();
    if (seen.find(current_qp) != seen.end()) continue;
    seen.insert(current_qp);
    if (current_qp.row == target.row && current_qp.col == target.col && current_qp.tool == Tools::TORCH) {
      return current_qp.cost;
    }
    auto new_mps = std::vector<MapPoint>();
    new_mps.emplace_back(add_point_to_map(map, current_qp.row+1, current_qp.col, depth));
    if (current_qp.row > 0) {
      new_mps.emplace_back(add_point_to_map(map, current_qp.row-1, current_qp.col, depth));
    }
    new_mps.emplace_back(add_point_to_map(map, current_qp.row, current_qp.col+1, depth));
    if (current_qp.col > 0) {
      new_mps.emplace_back(add_point_to_map(map, current_qp.row, current_qp.col-1, depth));
    }

    for (const auto& new_mp : new_mps) {
      const auto allowed_tools = get_allowed_tools(new_mp.region_type);
      if (allowed_tools[0] == current_qp.tool) {
        pq.emplace(QueuePoint(new_mp.row, new_mp.col, current_qp.cost + 1, current_qp.tool));
      } else if (allowed_tools[1] == current_qp.tool) {
        pq.emplace(QueuePoint(new_mp.row, new_mp.col, current_qp.cost + 1, current_qp.tool));
      }
    }
    const auto allowed_tools = get_allowed_tools(current_mp.region_type);
    if (current_qp.tool == allowed_tools[0]) {
      pq.emplace(QueuePoint(current_qp.row, current_qp.col, current_qp.cost + 7, allowed_tools[1]));
    } else {
      pq.emplace(QueuePoint(current_qp.row, current_qp.col, current_qp.cost + 7, allowed_tools[0]));
    }
  }
  return -1;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  std::getline(file, line);
  const int depth = std::stoi(line.substr(7, line.size() - 7));

  std::getline(file, line);
  constexpr auto delimiter = ',';
  const auto delimiter_index = line.find(delimiter);
  const auto target_col = std::stoi(line.substr(8, delimiter_index - 8));
  const auto target_row = std::stoi(line.substr(delimiter_index + 1, line.size() - delimiter_index));

  map2d map;

  const auto start = MapPoint(0, 0, 0, 0, RegionType::ROCKY);
  map.insert(start);

  auto target_temp = add_point_to_map(map, target_row, target_col, depth);
  map.erase(target_temp);
  target_temp.region_type = RegionType::ROCKY;
  const auto target = target_temp;
  map.insert(target);

  const auto time_cost = find_path(map, start, target, depth);
  std::cout << time_cost << '\n';
  return time_cost;
}
