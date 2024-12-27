#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits> 
#include <map>   
#include <unordered_map>   
#include <queue> 
#include <ranges>
#include <string> 
#include <utility> 
#include <vector>

constexpr int highest_keypad_idx = 26; // 0..26

struct Memory {
    char start;
    char end;
    int keypad_idx;

    bool operator == (const Memory& m) const {
        return start == m.start && end == m.end && keypad_idx == m.keypad_idx;
    }
};

struct MemoryHasher {
    std::size_t operator () (const Memory& m) const {
        return int(m.start) + int(m.end) * 100 + m.keypad_idx * 10000;
    }
};

std::unordered_map<Memory, long long int, MemoryHasher> memory;

struct Point {
    int row;
    int col;
    bool operator == (const Point& p) const {
        return row == p.row && col == p.col;
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

struct Comparator {
    bool operator()(
        const std::tuple<Point, std::vector<Point>, int>& p1, 
        const std::tuple<Point, std::vector<Point>, int>& p2) const {
        return std::get<2>(p1) > std::get<2>(p2);
    }
};

// Convert a given path to {'>','v','<','^'} chars
std::string convert_path_to_primitives(const std::vector<Point>& path) {
    const std::array<char, 4> primitives {{'>','v','<','^'}}; // order should match 'moves' defined above
    std::string path_primitives;
    for (int i = 0; i < path.size() - 1; i++) {
        const auto drow = path[i+1].row - path[i].row;
        const auto dcol = path[i+1].col - path[i].col;
        for (const auto& [idx,move] : std::views::enumerate(moves)) {
            if (move.row == drow && move.col == dcol) {
                path_primitives.push_back(primitives[idx]);
                break;
            }
        }
    }
    return path_primitives;
}

// Find all the best paths between a given pair of points, where 'P' (panic) os an obstacle 
std::vector<std::vector<Point>> find_best_paths(const std::vector<std::string>& map, const Point& start, const Point& end) {
    std::priority_queue<
        std::tuple<Point, std::vector<Point>, int>, 
        std::vector<std::tuple<Point, std::vector<Point>, int>>, 
        Comparator> pq;
    pq.push({start, {}, 0});
    std::vector<std::vector<Point>> best_paths;
    int best_score = std::numeric_limits<int>::max();
    while(!pq.empty()){
        auto [current, path, score] = pq.top();
        if (score > best_score) break;
        pq.pop();
        if (std::find(path.begin(), path.end(), current) != path.end()) continue;
        path.push_back(current);
        if (current.row == end.row && current.col == end.col) {
            best_score = score;
            best_paths.push_back(path);
        }
        for (const auto& move : moves) {
            Point new_point;
            new_point.row = current.row + move.row; 
            new_point.col = current.col + move.col;            
            if (!in_map(new_point, map)) {
                continue;
            }
            if (map[new_point.row][new_point.col] == 'P') {
                continue;
            }
            if (std::find(path.begin(), path.end(), new_point) != path.end()){ 
                continue;
            } 
            pq.push({new_point, path, score+1});
        }
    }
    return best_paths;
}  
const std::vector<std::string> num_keypad {
    { '7', '8', '9'},
    { '4', '5', '6'},
    { '1', '2', '3'},
    { 'P', '0', 'A'}
};

const std::vector<std::string> dir_keypad {
    { 'P', '^', 'A'},
    { '<', 'v', '>'}
};

// Best paths for each pair of points on the keypads
std::map<std::pair<char, char>, std::vector<std::string>> num_keypad_paths;
std::map<std::pair<char, char>, std::vector<std::string>> dir_keypad_paths;

std::map<std::pair<char, char>, std::vector<std::string>> find_all_best_paths_on_keypad (const std::vector<std::string>& keypad) {
    std::map<std::pair<char, char>, std::vector<std::string>> keypad_paths;
    for (const auto&  [row_l1_i, row_l1] : std::views::enumerate(keypad)) {
        for (const auto& [col_l1_i, ele_l1] : std::views::enumerate(row_l1)) {
            if (ele_l1 == 'P') continue;
            for (const auto&  [row_l2_i, row_l2] : std::views::enumerate(keypad)) {
                for (const auto& [col_l2_i, ele_l2] : std::views::enumerate(row_l2)) {
                    if (ele_l2 == 'P') continue;
                    const auto best_paths = find_best_paths(keypad, Point(row_l1_i, col_l1_i), Point(row_l2_i, col_l2_i));
                    for (const auto& best_path : best_paths) {
                        keypad_paths[{ele_l1, ele_l2}].push_back(convert_path_to_primitives(best_path));
                    }
               }
            }
        }
    }
    return keypad_paths;
}

long long dfs(const std::string& sequence, int keypad_idx) {
    if (keypad_idx == 0) {
        return sequence.size();
    }
    long long result = 0;
    const auto updated_sequence = 'A' + sequence;
    for (int i = 0; i < updated_sequence.size() - 1; i++) {
        Memory m{updated_sequence[i], updated_sequence[i+1], keypad_idx};
        if (memory.contains(m)) {
            result += memory[m];
        } else {
            long long answer = std::numeric_limits<long long>::max();
            for (const auto& v: dir_keypad_paths[{updated_sequence[i], updated_sequence[i+1]}]) {
                answer = std::min(answer, dfs(v + 'A', keypad_idx-1)); 
            }
            memory[m] = answer;
            result += answer;
        }
    }
    return result;
}

int main(int argc, char * argv []) {
    std::string input = "../input/day_21_input";
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    
    // Shortest paths on keypads
    num_keypad_paths = find_all_best_paths_on_keypad(num_keypad);
    dir_keypad_paths = find_all_best_paths_on_keypad(dir_keypad);
    long long sum_of_complexities = 0;
    std::string code;
    while(std::getline(file, code)) {
        code = 'A' + code;
        long long best_length = 0;
        for (int i = 0; i < code.size() - 1; i++) {
            long long best_length_consecutive = std::numeric_limits<long long>::max();
            for (const auto& v: num_keypad_paths[{code[i], code[i+1]}]) {
                best_length_consecutive = std::min(best_length_consecutive, dfs(v + 'A', highest_keypad_idx-1)); // -1 since first keypad is already being used in this loop
            }
            best_length += best_length_consecutive;
        }
        const int num_val = std::stoi(code.substr(1, code.size() - 2));
        sum_of_complexities += best_length * num_val;
    }
    std::cout << sum_of_complexities << std::endl;
}