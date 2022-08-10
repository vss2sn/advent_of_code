#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <numeric>

void print(const std::vector<int>& v) {
  std::cout << '|';
  for(const auto& ele : v) {
    std::cout << ele << '|';
  }
  std::cout << '\n';
}

int dist(const std::vector<int>& p1, const std::vector<int>& p2) {
  int d = 0;
  for (int i = 0; i < p1.size(); i++) {
    d += std::abs(p1[i] - p2[i]);
  }
  return d;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_25_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  const std::string delimiter = ",";
  std::vector<std::vector<int>> points;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line.size() == 0) continue;
    std::vector<int> point;
    size_t start = 0;
    size_t end = line.find(delimiter, start);
    while(end != std::string::npos) {
      point.push_back(std::stoi(line.substr(start, end - start)));
      start = end + delimiter.size();
      end = line.find(delimiter, start);
    }
    point.push_back(std::stoi(line.substr(start, line.size() - start)));
    points.push_back(point);
  }

  std::vector<int> parent(points.size(), -1);
  for (int i = 0; i < points.size(); i++) {
    if (parent[i] == -1) parent[i] = i;
    for (int j = i+1; j < points.size(); j++) {
      if (dist(points[i], points[j]) <= 3) {
        if (parent[i] == parent[j]) continue;
        else if (parent[i] != -1 && parent[j] == -1) parent[j] = parent[i];
        else if (parent[i] == -1 && parent[j] != -1) parent[i] = parent[j];
        else {
          const auto to_replace = parent[j];
          for (auto& ele : parent) {
            if (ele == to_replace) ele = parent[i];
          }
        }
      }
    }
  }
  std::unordered_set<int> s;
  for(const auto ele : parent) {
    s.insert(ele);
  }
  std::cout << s.size() << '\n';
  return 0;
}
