#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

void update_boards(std::vector<std::vector<int>>& boards, const int number) {
  for (auto& board : boards) {
    if (const auto it = std::find(std::begin(board), std::end(board), number); it != board.end()) {
      *it = -1;
    }
  }
}

int update_boards_to_ignore(const std::vector<std::vector<int>>& boards, std::unordered_set<int>& to_ignore) {
  int last_board = -1;
  for (int i_board = 0; i_board < boards.size(); i_board++) {
    if (to_ignore.find(i_board) != to_ignore.end()) continue;
    std::vector<bool> found(10, true);
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        if (boards[i_board][i * 5 + j] != -1) {
          found[i] = false;
        }
        if (boards[i_board][j * 5 + i] != -1) {
          found[i + 5] = false;
        }
      }
    }
    if (const auto it = std::find(std::begin(found), std::end(found), true); it != std::end(found)) {
      if (const auto p = to_ignore.insert(i_board); p.second) {
        last_board = i_board;
      }
    }
  }
  return last_board;
}

inline std::vector<int> split_string_to_vec(const std::string& s, const std::string& delimiter) {
  std::size_t start = 0;
  std::size_t end = s.find(delimiter);
  std::vector<int> v;
  while (end != std::string::npos) {
    v.emplace_back(std::stoi(s.substr(start, end - start)));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
  }
  v.emplace_back(std::stoi(s.substr(start, s.size() - start)));
  return v;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_04_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  const std::vector<int> numbers = split_string_to_vec(line, ",");
  std::vector<std::vector<int>> boards;
  while(std::getline(file, line)) {
    boards.emplace_back();
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        int n;
        file >> n;
        boards.back().emplace_back(n);
      }
    }
  }

  // Needed due to the way the file is being read
  // Editor adds extra last line
  boards.erase(std::begin(boards) + boards.size() - 1);

  std::unordered_set<int> to_ignore;
  for (const auto number : numbers) {
    update_boards(boards, number);
    int last_board = update_boards_to_ignore(boards, to_ignore);
    if (boards.size() == to_ignore.size()) {
      const auto sum = std::accumulate(std::begin(boards[last_board]), std::end(boards[last_board]), 0, [](const auto sum, const auto ele) { return ele != -1 ? sum + ele : sum; });
      std::cout << sum * number << '\n';
      return 0;
    }
  }
  return 0;
}
