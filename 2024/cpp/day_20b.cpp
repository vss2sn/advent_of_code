#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <queue> 
#include <ranges>
#include <string>
#include <utility> 
#include <vector> 

// Note:
// * There is only a single path from start to finish
// * Cheats do not stop as soon as programs exit from the walls; 
//   programs can re-enter walls as long as the number of steps taken
//   does not exceed the maximum number of steps

constexpr int n_steps = 20;

struct Point {
    int row;
    int col;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col;
    }
    
    bool operator < (const Point& p) const {
        return row < p.row || (row == p.row &&  col < p.col);
    }
};

constexpr std::array<Point, 4> moves {
    Point( 0, 1),
    Point( 1, 0),
    Point( 0,-1),
    Point(-1, 0)
};

bool in_map(const Point& p, const std::vector<std::string>& map ) {
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[0].size();
}

std::pair<std::set<Point>, std::vector<std::vector<int>>> find_path (
    const std::vector<std::string>& map, 
    const Point& start, 
    const Point& end
) {
    std::queue<std::pair<Point, int>> q;
    std::set<Point> visited;
    std::vector<std::vector<int>> cost_on_path(map.size(), std::vector<int>(map[0].size(), 0));
    q.push({end, 0});
    while(!q.empty()){
        const auto [current, score] = q.front();
        q.pop();
        if (visited.contains(current)) continue;
        visited.insert(current);
        cost_on_path[current.row][current.col] = score;
        if (start.row == current.row && start.col == current.col) {
            return {visited, cost_on_path};
        }
        for (const auto& move : moves) {
            Point new_point;
            new_point.row = current.row + move.row; 
            new_point.col = current.col + move.col; 
            if (!in_map(new_point, map)) {
                continue;
            } else if (map[new_point.row][new_point.col] == '#') {
                continue;
            } else {
                q.push({new_point, score + 1});
            }
        }
    }
    return {{},{}};
}

std::map<int, int> find_possible_cheats(
    const std::vector<std::string>& map,
    const std::vector<std::vector<int>>& costs_on_path,
    const std::set<Point>& points_on_path,
    const Point& start,
    const Point& end,
    const int steps
) {
    std::map<int, int> cheats_count;
    for (auto p : points_on_path) {
        const int cost_at_p = costs_on_path[p.row][p.col];
        for (int d_row = -steps; d_row <= steps; d_row++) {
            for (int d_col = -steps+std::abs(d_row); d_col <= +steps-std::abs(d_row); d_col++) {
                const auto ep = Point(p.row + d_row, p.col + d_col);
                if (!points_on_path.contains(ep)) continue;
                const int cost_at_ep = costs_on_path[ep.row][ep.col];
                const auto time_saved = cost_at_p - cost_at_ep - std::abs(d_row) - std::abs(d_col);
                if (time_saved) {
                    cheats_count[time_saved]++;
                }
            }
        }
    }
    return cheats_count;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_20_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<std::string> map;
    Point start;
    Point end;
    while(std::getline(file, line)) {
        map.push_back(line);
        for (const auto [idx,ele] : std::views::enumerate(line)) {
            if (ele == 'S') {
                start.row = map.size()-1;
                start.col = idx;
            }
            else if (ele == 'E') {
                end.row = map.size()-1;
                end.col = idx;
            }
        }
    }
    const auto [points_on_path, costs_on_path] = find_path (map, start, end);
    const auto cheats_map = find_possible_cheats(map, costs_on_path, points_on_path, start, end, n_steps);
    int count = 0;
    for (const auto& [val, n] : cheats_map) {
        // if (val >= 50) std::cout << "There are " << n << " cheats that save " << val << " picoseconds" << '\n';
        if (val >= 100) count += n;
    }
    std::cout << count << '\n';
    return 0;
}
