#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

int GCD(const int a, const int b) {
  if (a == 0) {
    return b;
  }
  return GCD(b % a, a);
}

std::vector<std::pair<int, int>> remove_all_dupes(
    std::vector<std::pair<int, int>> const& input) {
  if (input.size() < 2) return input;
  std::vector<std::pair<int, int>> ret;
  ret.push_back(input[0]);
  for (int i = 1; i < input.size(); ++i) {
    if (ret.back() != input[i]) {
      ret.push_back(input[i]);
    } else {
      std::cout << "Removing " << '\n';
    }
  }
  return ret;
}

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_10_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::vector<std::string> asteroid_map;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line),
                              [](auto c) { return !isprint(c); }),
               std::end(line));
    if (line.empty()) break;
    asteroid_map.emplace_back(line);
  }

  const int x_max = asteroid_map[0].size();
  const int y_max = asteroid_map.size();

  std::unordered_map<
      std::pair<int, int>,
      std::unordered_map<std::pair<int, int>, std::pair<int, int>, hash_pair>,
      hash_pair>
      los;

  std::vector<std::pair<int, int>> slopes;
  std::unordered_set<std::pair<int, int>, hash_pair> slopes_set;
  for (int dx = 0; dx < x_max; dx++) {
    for (int dy = dx + 1; dy < y_max; dy++) {
      const auto gcd = GCD(dx, dy);
      const auto ndx = dx / gcd;
      const auto ndy = dy / gcd;
      if (slopes_set.find({ndx, ndy}) == slopes_set.end()) {
        slopes_set.insert({ndx, ndy});
        slopes.emplace_back(ndx, ndy);
        slopes.emplace_back(ndx, -ndy);
        slopes.emplace_back(-ndx, ndy);
        slopes.emplace_back(-ndx, -ndy);
        slopes.emplace_back(ndy, ndx);
        slopes.emplace_back(ndy, -ndx);
        slopes.emplace_back(-ndy, ndx);
        slopes.emplace_back(-ndy, -ndx);
      }
    }
  }
  auto toAngle = [](const std::pair<int, int> a) {
    double angle = std::atan2(double(a.second), double(a.first));
    return (angle >= 0.0) ? angle : (2.0 * M_PI + angle);
  };
  auto sorting_function = [&toAngle](const auto s1, const auto s2) {
    return toAngle(s1) < toAngle(s2);
  };
  std::sort(std::begin(slopes), std::end(slopes), sorting_function);
  slopes = remove_all_dupes(slopes);

  int max_count = 0;
  std::pair<int, int> best_loc;
  for (int x = 0; x < x_max; x++) {
    for (int y = 0; y < y_max; y++) {
      if (asteroid_map[x][y] != '#') continue;
      int count = 0;
      const std::pair<int, int> coord{x, y};
      for (const auto& slope : slopes) {
        auto new_coord = std::make_pair(coord.first + slope.first,
                                        coord.second + slope.second);
        while (new_coord.first >= 0 && new_coord.first < x_max &&
               new_coord.second >= 0 && new_coord.second < y_max) {
          if (asteroid_map[new_coord.first][new_coord.second] == '#') {
            count++;
            break;
          }
          new_coord.first += slope.first;
          new_coord.second += slope.second;
        }
      }
      if (count > max_count) {
        max_count = count;
        best_loc = coord;
      }
    }
  }

  std::cout << max_count << '\n';
  std::cout << best_loc.first << ' ' << best_loc.second << '\n';
  return max_count;
}
