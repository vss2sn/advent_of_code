#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue> 
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>


struct Storm {
    Storm(const int period, const int offset) : period(period), offset(offset) {}
    int period;
    int offset;

    friend std::ostream& operator<<(std::ostream& os, const Storm& p);
};

std::ostream& operator<<(std::ostream& os, const Storm& p) {
    os << '(' << p.period << ' ' << p.offset << ')';
    return os;
}

struct State {
    State (const int row = 0, const int col = 0, const int time = 0) : row(row), col(col), time(time) {}
    int row; 
    int col;
    int time;
    bool operator == (const State& p) const {
        return p.row == row && p.col == col && p.time == time;
    }

    friend std::ostream& operator<<(std::ostream& os, const State& p);
};

struct CompareStateTime {
    bool operator () (const State& s1, const State& s2) const {
        return s1.time < s2.time;
    }
};

struct StateHash {
    std::size_t operator () (const State& p) const {
    return (p.row * 10) + p.col;
    }
};

std::ostream& operator<<(std::ostream& os, const State& p) {
    os << '(' << p.row << ' ' << p.col << ' ' << p.time << ')';
    return os;
}

State find_path(const State& start, const State& end, const std::vector<std::vector<std::vector<Storm>>>& time_offsets, const std::vector<std::string>& grid) {
    
    const int n_rows = time_offsets.size();
    const int n_cols = time_offsets[0].size();

    const std::array<State, 5> moves = {{
        State( 0,  1),
        State( 1,  0),
        State( 0, -1),
        State(-1,  0),
        State( 0,  0)
    }};
    const auto in_bounds = [&n_rows, &n_cols, &grid](const State& s) {
        return s.row >= 0 && s.col >=0 && s.row < n_rows && s.col < n_cols && grid[s.row][s.col] != '#'; 
    };
    const auto is_storm = [&time_offsets](const State& s) {
        for (const auto storm : time_offsets[s.row][s.col]) {
            if (s.time % storm.period == storm.offset) {
                return true;
            }
        }
        return false;
    };

    std::queue<State> q;
    q.push(start);
    std::unordered_set<State, StateHash> visited;
    while(!q.empty()) {
        const auto current = q.front();
        q.pop();
        if (visited.find(current) != visited.end()) continue;
        if (current.time > (n_cols - 2) * (n_rows - 2)) {
            auto temp = current;
            temp.time = temp.time % ((n_cols - 2) * (n_rows - 2));
            if (visited.find(temp) != visited.end()) continue;
        }
        visited.insert(current);

        if (current.row == end.row  && current.col == end.col) {
            std::cout << current.time << '\n';
            return current;
        }
        for (const auto move : moves) {
            const auto new_state = State (current.row + move.row, current.col + move.col, current.time + 1);
            // std::cout << "New State: " << new_state << " ---> ";
            if (!in_bounds(new_state)) {
                // std::cout << "out of bounds" << '\n';
                continue;
            }
            if (is_storm(new_state)) {
                // std::cout << "is a storm" << '\n';
                continue;
            }
            // std::cout << " is acceptable" << '\n';
            q.push(new_state);
        }
    }
    std::cout << "Could not find path" << '\n';
    return State();
 
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_24_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::fstream file(input);

    std::vector<std::string> grid;

    while(std::getline(file, line)) {
        // std::cout << '|' << line << '|' << '\n';
        grid.push_back(line);
    }

    const int n_rows = grid.size();
    const int n_cols = grid[0].size();

    // std::cout << n_rows << ' ' << n_cols << '\n';
    std::vector<std::vector<std::vector<Storm>>> time_offsets(n_rows, std::vector<std::vector<Storm>>(n_cols));

    for (int row = 1; row < n_rows-1; row++) {
        for (int col = 1; col < n_cols-1; col++) {
            // std::cout << row << ' ' << col << '\n';
            if (grid[row][col] == '^') {
                // std::cout << grid[row][col] << '\n';
                const auto period = n_rows - 2;
                // std::cout << period << '\n';
                int offset = 0;
                for (int i = row; i > 0; i--, offset++) {
                    time_offsets[i][col].emplace_back(Storm(period, offset));
                }
                for (int i = n_rows-1-1; i > row; i--, offset++) {
                    time_offsets[i][col].emplace_back(Storm(period, offset));
                }
            }
            else if (grid[row][col] == 'v') {
                // std::cout << grid[row][col] << '\n';
                const auto period = n_rows - 2;
                // std::cout << period << '\n';
                int offset = 0;
                for (int i = row; i < (n_rows-1); i++, offset++) {
                    time_offsets[i][col].emplace_back(Storm(period, offset));
                }
                for (int i = 1; i < row; i++, offset++) {
                    time_offsets[i][col].emplace_back(Storm(period, offset));
                }
            }
            else if (grid[row][col] == '>') {
                // std::cout << grid[row][col] << '\n';
                const auto period = n_cols - 2;
                // std::cout << period << '\n';
                int offset = 0;
                for (int i = col; i < (n_cols-1); i++, offset++) {
                    time_offsets[row][i].emplace_back(Storm(period, offset));
                }
                for (int i = 1; i < col; i++, offset++) {
                    time_offsets[row][i].emplace_back(Storm(period, offset));
                }
            }
            else if (grid[row][col] == '<') {
                // std::cout << grid[row][col] << '\n';
                const auto period = n_cols - 2;
                // std::cout << period << '\n';
                int offset = 0;
                for (int i = col; i > 0; i--, offset++) {
                    time_offsets[row][i].emplace_back(Storm(period, offset));
                }
                for (int i = n_cols-1-1; i > col; i--, offset++) {
                    time_offsets[row][i].emplace_back(Storm(period, offset));
                }
            }
        }
    }

    const auto state_at_goal = find_path(State(0, 1, 0), State(n_rows-1, n_cols-2, 0), time_offsets, grid);
    std::cout << state_at_goal << '\n';
    const auto state_back_at_start = find_path(state_at_goal, State(0, 1, 0), time_offsets, grid);
    std::cout << state_back_at_start << '\n';
    const auto state_back_at_goal = find_path(state_back_at_start, State(n_rows-1, n_cols-2, 0), time_offsets, grid);
    std::cout << state_back_at_goal << '\n';
    std::cout << state_back_at_goal.time << '\n';
    return 0;
}