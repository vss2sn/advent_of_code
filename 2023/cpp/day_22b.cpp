#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cassert>


struct Brick {
  std::array<int, 3> start {0,0,0};
  std::array<int, 3> end {0,0,0};
  int id = 0;
  bool operator == (const Brick& b) const {
    return b.start == start && b.end == end;
  }
};

bool check_if_overlap(const Brick& b1, const Brick& b2) {
  bool x_overlap = false;
  bool y_overlap = false;
  x_overlap = std::min(b2.start[0], b2.end[0]) <= b1.start[0] && b1.start[0] <= std::max(b2.start[0], b2.end[0]);
  y_overlap = std::min(b2.start[1], b2.end[1]) <= b1.start[1] && b1.start[1] <= std::max(b2.start[1], b2.end[1]);
  if (x_overlap && y_overlap) return true;
  x_overlap = x_overlap || std::min(b2.start[0], b2.end[0]) <= b1.end[0] && b1.end[0] <= std::max(b2.start[0], b2.end[0]);
  y_overlap = y_overlap || std::min(b2.start[1], b2.end[1]) <= b1.end[1] && b1.end[1] <= std::max(b2.start[1], b2.end[1]);
  if (x_overlap && y_overlap) return true;
  x_overlap = x_overlap || std::min(b1.start[0], b1.end[0]) <= b2.start[0] && b2.start[0] <= std::max(b1.start[0], b1.end[0]);
  y_overlap = y_overlap || std::min(b1.start[1], b1.end[1]) <= b2.start[1] && b2.start[1] <= std::max(b1.start[1], b1.end[1]);
  if (x_overlap && y_overlap) return true;
  x_overlap = x_overlap || std::min(b1.start[0], b1.end[0]) <= b2.end[0] && b2.end[0] <= std::max(b1.start[0], b1.end[0]);
  y_overlap = y_overlap || std::min(b1.start[1], b1.end[1]) <= b2.end[1] && b2.end[1] <= std::max(b1.start[1], b1.end[1]);
  if (x_overlap && y_overlap) return true;
  return false;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  const std::regex mask_pattern(R"((-?[0-9]+),(-?[0-9]+),(-?[0-9]+)~(-?[0-9]+),(-?[0-9]+),(-?[0-9]+))");
  std::smatch mask_match;
  std::vector<Brick> bricks;
  while(std::getline(file, line)) {
    std::regex_search(line, mask_match, mask_pattern); 
    Brick b;
    b.id = bricks.size();
    b.start[0] = std::stoi(mask_match[1]);
    b.start[1] = std::stoi(mask_match[2]);
    b.start[2] = std::stoi(mask_match[3]);
    b.end[0] = std::stoi(mask_match[4]);
    b.end[1] = std::stoi(mask_match[5]);
    b.end[2] = std::stoi(mask_match[6]);
    bricks.push_back(b);
    // std::cout << b.start[0] << ' ' << b.start[1] << ' ' 
    //           << b.start[2] << ' ' << b.end[0] << ' ' 
    //           << b.end[1]<< ' ' << b.end[2] << '\n';
  }
  for (auto& b : bricks) {
    if (b.start[2] > b.end[2]) {
      std::swap(b.start, b.end);
    }
  }
  std::sort(
    std::begin(bricks), 
    std::end(bricks), 
    [](const auto& b1, const auto& b2) { 
      return b1.start[2] < b2.start[2];
    }
  );
  {  
    const auto h = bricks[0].end[2] - bricks[0].start[2];
    bricks[0].start[2] = 1;
    bricks[0].end[2] = 1 + h;
  }
  for (int i = 1; i < bricks.size(); i++) {
    // std::cout << "\nBrick moving down: " << char('A' + bricks[i].id) << '\n';
    const auto h = bricks[i].end[2] - bricks[i].start[2];
    for (int j = i - 1; j >= 0; j--) {
    // std::cout << "Comparing with: " << char('A' + bricks[j].id) << '\n';
      bricks[i].start[2] = bricks[j].start[2];
      bricks[i].end[2] = bricks[i].start[2] + h;
      if (check_if_overlap(bricks[i], bricks[j])) {
        // std::cout << "Overlap found between " << char('A' + bricks[i].id) << " and " << char('A' + bricks[j].id) << '\n';
        // std::cout << char('A' + bricks[j].id) << " occupies " << bricks[j].start[2] << "->" <<  bricks[j].end[2]  << " in Z" << '\n';
        bricks[i].start[2] = bricks[j].end[2] + 1;
        bricks[i].end[2] = bricks[i].start[2] + h;
        // std::cout << char('A' + bricks[i].id) << " now set to " << bricks[i].start[2] << "->" <<  bricks[i].end[2]  << '\n';
        break;
      }
    }
    // While the bricks are arranged in z bfore they start falling, 
    // they might have a differnt height order after they land
    std::partial_sort(
      std::begin(bricks), 
      std::next(std::begin(bricks), i), 
      std::end(bricks), 
      [](const auto& b1, const auto& b2) { 
        return b1.end[2] < b2.end[2];
      }
    );
  }
  std::array<int, 3> values {0,0,0};
  for (const auto& b : bricks) {
    values[0] = std::max(values[0], b.start[0]);
    values[0] = std::max(values[0], b.end[0]);
    values[1] = std::max(values[1], b.start[1]);
    values[1] = std::max(values[1], b.end[1]);
    values[2] = std::max(values[2], b.start[2]);
    values[2] = std::max(values[2], b.end[2]);
  }
  std::vector<std::vector<std::vector<int>>> map (
    values[0]+1, std::vector<std::vector<int>>(
      values[1]+1, std::vector<int>(
        values[2]+1, 0
      )
    )
  );
  // std::cout << values[0] << ' ' << values[1] << ' ' << values[2] << '\n';
  for (const auto& b : bricks) {
    for (int x = std::min(b.start[0],b.end[0]); x <= std::max(b.start[0],b.end[0]); x++) {
      for (int y = std::min(b.start[1], b.end[1]); y <= std::max(b.start[1],b.end[1]); y++) {
        for (int z = std::min(b.start[2], b.end[2]); z <= std::max(b.start[2],b.end[2]); z++) {
          // std::cout << x << ' ' << y << ' ' << z << '\n';
          map[x][y][z] = b.id;
        }
      }
    }
  }
  
  // for (int z = 0; z <= values[2]; z++) {
  //   for (int x = 0; x <= values[0]; x++) {
  //     std::cout << map[x][0][z];
  //   }
  //   std::cout << '\n';
  // }
  
  std::unordered_map<int, std::vector<int>> supports;
  std::unordered_map<int, std::vector<int>> supported_by;
  for(const auto& b : bricks) {
    supports[b.id] = {};
    supported_by[b.id] = {};
  }
  for (int i = 0; i < bricks.size(); i++) {
    for (int j = 0; j < bricks.size(); j++) {
      if (i == j) continue;
      if(bricks[i].end[2] + 1 == bricks[j].start[2] && check_if_overlap(bricks[i], bricks[j])) {
        supports[bricks[i].id].push_back(bricks[j].id);
        supported_by[bricks[j].id].push_back(bricks[i].id);
      }
    }
  }
  std::size_t count = 0;
  for (const auto& b : bricks) {
    std::queue<int> q;
    for (const auto& ele : supports[b.id]) {
      q.push(ele);
    }
    // Do not cache bricks that might have been seen as 
    // they are added each time one of the bricks that supports them is destroyed
    // Only the last time they are added, 
    // at which point all the bricks supporting them might have been destroyed
    // will they be free fall and hence be counted
    std::unordered_set<int> moved {b.id};
    while(!q.empty()) {
      const auto current = q.front();
      q.pop();
      bool will_move = true;
      for (const auto& ele : supported_by[current]) {
        if (moved.find(ele) == moved.end()) {
          will_move = false;
          break;
        }
      }
      if (!will_move) continue;
      moved.insert(current);
      for (const auto& supported_by_current : supports[current]) {
        q.push(supported_by_current);
      }
    }
    count += moved.size() - 1;
  }
  std::cout << count << '\n';
  return 0;
}