#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

std::vector<std::pair<int, int>> getWireVector(const std::string& wire_str) {
  const std::string delimiter = ",";
  std::size_t start = 0;
  std::size_t end = wire_str.find(delimiter);
  std::vector<std::pair<int, int>> wire;
  std::pair<int, int> cur_coord{0, 0};
  while (end != std::string::npos) {
    const auto instr = wire_str.substr(start, end - start);
    if (instr[0] == 'U') {
      for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
        cur_coord.second += 1;
        wire.push_back(cur_coord);
      }
    } else if (instr[0] == 'R') {
      for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
        cur_coord.first += 1;
        wire.push_back(cur_coord);
      }
    } else if (instr[0] == 'L') {
      for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
        cur_coord.first -= 1;
        wire.push_back(cur_coord);
      }
    } else if (instr[0] == 'D') {
      for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
        cur_coord.second -= 1;
        wire.push_back(cur_coord);
      }
    }
    start = end + delimiter.size();
    end = wire_str.find(delimiter, start);
  }
  const auto instr = wire_str.substr(start, end);
  if (instr[0] == 'U') {
    for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
      cur_coord.second += 1;
      wire.push_back(cur_coord);
    }
  } else if (instr[0] == 'R') {
    for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
      cur_coord.first += 1;
      wire.push_back(cur_coord);
    }
  } else if (instr[0] == 'L') {
    for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
      cur_coord.first -= 1;
      wire.push_back(cur_coord);
    }
  } else if (instr[0] == 'D') {
    for (int i = 0; i < std::stoi(instr.substr(1, instr.size() - 1)); ++i) {
      cur_coord.second -= 1;
      wire.push_back(cur_coord);
    }
  }
  return wire;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_str1, input_str2;
  std::getline(file, input_str1);
  std::getline(file, input_str2);
  std::vector<std::pair<int, int>> wire1 = getWireVector(input_str1);
  std::vector<std::pair<int, int>> wire2 = getWireVector(input_str2);

  // Solve
  std::set<std::pair<int, int>> wire2set;
  for (const auto& coord : wire2) {
    wire2set.insert(coord);
  }

  size_t time_delay = wire1.size();
  for (size_t index1 = 0; index1 < wire1.size(); index1++) {
    if (wire2set.find(wire1[index1]) != wire2set.end()) {
      const auto it =
          std::find(std::begin(wire2), std::end(wire2), wire1[index1]);
      size_t index2 = std::distance(std::begin(wire2), it);
      time_delay =
          std::min(time_delay,
                   index1 + index2 + 2);  // Center not included in wire length
    }
  }

  std::cout << time_delay << '\n';
  return time_delay;
}
