#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

void update_boards(std::vector<std::vector<int>>& boards, const int number) {
  for (auto& board : boards) {
    if (const auto it = std::find(std::begin(board), std::end(board), number); it != board.end()) {
      *it = -1;
    }
  }
}

std::tuple<bool, int> check_boards(const std::vector<std::vector<int>>& boards) {
  for (int i_board = 0; i_board < boards.size(); i_board++) {
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
    if (std::find(std::begin(found), std::end(found), true) != std::end(found)) {
      return {true, i_board};
    }
  }
  return {false, -1};
}

std::vector<int> split_string_to_vec(const std::string& s, const std::string& delimiter) {
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

  for (const auto number : numbers) {
    update_boards(boards, number);
    const auto [found, i_board] = check_boards(boards);
    if (found) {
      const auto sum = std::accumulate(std::begin(boards[i_board]), std::end(boards[i_board]), 0, [](const auto sum, const auto ele) { return ele != -1 ? sum + ele : sum; });
      std::cout << sum * number << '\n';
      return 0;
    }
  }

  return 0;
}
