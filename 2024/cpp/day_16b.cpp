#include <array>
#include <fstream>
#include <iostream>
#include <limits> 
#include <queue> 
#include <ranges>
#include <string>
#include <tuple> 
#include <unordered_map> 
#include <unordered_set>
#include <utility> 
#include <vector> 

// Find the mimimum cost of each point (x, y, dir) from the end point
// From the start point add every point into a set where current_point's cost + cost of motion = next_points's optimal cost

struct Point {
    int row;
    int col;
    int dir;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col && dir == p.dir;
    }
    
    bool operator < (const Point& p) const {
        return row < p.row || (row == p.row &&  col < p.col)  || (row == p.row &&  col == p.col && dir < p.dir);
    }
};

struct SamePositionChecker {
    bool operator() (const Point& p1, const Point& p2) const {
        return p1.row == p2.row && p1.col == p2.col;
    }
};

constexpr std::array<Point, 4> moves {
    Point( 0, 1, 0),
    Point( 1, 0, 1),
    Point( 0,-1, 2),
    Point(-1, 0, 3)
};

struct Hasher {
    std::size_t operator () (const Point& p) const {
        return p.row;
    }
};

bool in_map(const Point& p, const std::vector<std::string>& map ) {
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[0].size();
}

struct Comparator {
    bool operator()(const std::pair<Point, int>& p1, const std::pair<Point, int>& p2) const {
        return p1.second > p2.second;
    }
};

struct ComparatorMax {
    bool operator()(const std::pair<Point, int>& p1, const std::pair<Point, int>& p2) const {
        return p1.second < p2.second;
    }
};

std::vector<std::vector<std::array<int, 4>>> find_best_scores_for_all_points(
    const std::vector<std::string>& map,
    const Point& start,
    const Point& end,
    const int best_path_score
    ) {
    std::vector<std::vector<std::array<int, 4>>> best_cost_map(
        map.size(), 
        std::vector<std::array<int, 4>>(
            map[0].size(), 
            {{
                std::numeric_limits<int>::min(),
                std::numeric_limits<int>::min(),
                std::numeric_limits<int>::min(),
                std::numeric_limits<int>::min()
            }}
        ) 
    );
    std::priority_queue<std::pair<Point, int>, std::vector<std::pair<Point, int>>, Comparator> pq;
    std::unordered_set<Point, Hasher> visited;
    pq.push({end, 0});
    while(!pq.empty()){
        const auto [current, score] = pq.top();
        pq.pop();
        if (visited.contains(current)) continue;
        visited.insert(current);
        best_cost_map[current.row][current.col][current.dir] = score;
        const auto& move = moves[current.dir];
        Point new_point;
        new_point.row = current.row - move.row; 
        new_point.col = current.col - move.col;
        if (!in_map(new_point, map)) {
            continue;
        }
        if (map[new_point.row][new_point.col] == '#') {
            continue;
        }
        for (int dir = 0; dir < 4; dir++) {
            new_point.dir = dir;
            if (current.dir == new_point.dir) {
                pq.push({new_point, score + 1});
            } else if (std::abs(current.dir - new_point.dir) == 2) {
                pq.push({new_point, score + 2001});
            } else {
                pq.push({new_point, score + 1001});
            }
        }
    }
    return best_cost_map;
}

std::pair<int, int> find_best_path_score (const std::vector<std::string>& map, const Point& start, const Point& end) {
    std::priority_queue<std::pair<Point, int>, std::vector<std::pair<Point, int>>, Comparator> pq;
    std::unordered_set<Point, Hasher> visited;
    pq.push({start, 0});
    int best_path_score = 0;
    while(!pq.empty()){
        const auto [current, score] = pq.top();
        pq.pop();
        if (visited.contains(current)) continue;
        visited.insert(current);
        if (current.row == end.row && current.col == end.col) {
            return {score, current.dir};// score, end dir
        }
        for (const auto& move : moves) {
            Point new_point;
            new_point.row = current.row + move.row; 
            new_point.col = current.col + move.col;
            new_point.dir = move.dir;
            
            if (!in_map(new_point, map)) {
                continue;
            }
            else if (map[new_point.row][new_point.col] == '#') {
                continue;
            }
            else { //(map[new_point.row][new_point.col] == '.') {
                if (current.dir == new_point.dir) {
                    pq.push({new_point, score + 1});
                } else if (std::abs(current.dir - new_point.dir) == 2) {
                    pq.push({new_point, score + 2001});
                } else {
                    pq.push({new_point, score + 1001});
                }
            }
        }
    }
    return {-1, -1};
}

std::unordered_set<Point, Hasher> traverse_only_best_paths(
    const std::vector<std::vector<std::array<int, 4>>>& best_scores_map,
    const std::vector<std::string>& map,
    const Point& start,
    const Point& end
) {
    std::priority_queue<std::pair<Point, int>, std::vector<std::pair<Point, int>>, Comparator> pq;
    std::unordered_set<Point, Hasher> visited;
    pq.push({start, best_scores_map[start.row][start.col][start.dir]});
    while(!pq.empty()){
        const auto [current, score] = pq.top();
        pq.pop();
        if (visited.contains(current)) continue;
        visited.insert(current);
        if (current.row == end.row && current.col == end.col) {
            continue;
        }
        for (const auto& move : moves) {
            Point new_point;
            new_point.row = current.row + move.row; 
            new_point.col = current.col + move.col;
            new_point.dir = move.dir;
            if (!in_map(new_point, map)) {
                continue;
            }
            else if (map[new_point.row][new_point.col] == '#') {
                continue;
            }
            else {
                if (current.dir == new_point.dir && best_scores_map[new_point.row][new_point.col][new_point.dir] == score - 1) {
                    pq.push({new_point, score - 1});
                } else if (std::abs(current.dir - new_point.dir) == 2 && best_scores_map[new_point.row][new_point.col][new_point.dir] == score - 2001) {
                    pq.push({new_point, score - 2001});
                } else if ((std::abs(current.dir - new_point.dir) == 1 || std::abs(current.dir - new_point.dir) == 3) && best_scores_map[new_point.row][new_point.col][new_point.dir] == score - 1001) {
                    pq.push({new_point, score - 1001});
                }
            }
        }
    }
    return visited;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_16_input";
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
                start.dir = 0;
            }
            else if (ele == 'E') {
                end.row = map.size()-1;
                end.col = idx;
            }
        }
    }
    // Assumes that the end position is reached optimally from one direction only
    const auto [best_path_score, end_dir] = find_best_path_score(map, start, end);
    end.dir = end_dir;
    const auto best_score_map = find_best_scores_for_all_points(map, start, end, best_path_score);
    const auto on_best_path = traverse_only_best_paths(best_score_map, map, start, end);
    std::unordered_set<Point, Hasher, SamePositionChecker> on_best_path_filtered; // remove duplicates that are the same location but have a different direction
    for (const auto& p : on_best_path) {
        on_best_path_filtered.insert(p);
    }
    // for (const auto& p : on_best_path_filtered) {
    //     map[p.row][p.col] = 'O';
    // }
    // for (const auto & row : map) {
    //     std::cout << row << '\n';
    // }
    std::cout << on_best_path_filtered.size() << '\n';
    return 0;
}