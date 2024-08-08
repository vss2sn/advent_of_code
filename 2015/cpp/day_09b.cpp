#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex> 
#include <string>
#include <vector>

struct Distance {
    std::string from;
    std::string to;
    int value;
};

void util (int current, int& max_distance, int distance, std::vector<bool>& visited, const std::vector<std::vector<int>>& am) {
    if (std::all_of(visited.begin(), visited.end(), [](const auto ele ) {return ele;})) {
        max_distance = std::max(distance, max_distance);
        return;
    }
    for (int i = 0; i < visited.size(); i++) {
        if (am[current][i] == std::numeric_limits<int>::min()) continue;
        if (visited[i]) continue;
        visited[i] = true;
        distance += am[current][i];
        util(i, max_distance, distance, visited, am);
        distance -= am[current][i];
        visited[i] = false;
    }
}

int find_longest_path (const std::vector<std::string>& cities, const std::vector<std::vector<int>>& am) {
    std::vector<bool> visited(cities.size(), false);
    int max_distance = std::numeric_limits<int>::min();
    int distance = 0;
    for (int start_city = 0; start_city < cities.size(); start_city++) {
        visited[start_city] = true;
        util(start_city, max_distance, distance, visited, am);
        visited[start_city] = false;
    }
    return max_distance;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::vector<Distance> distances;
  const std::regex pattern(R"(([a-zA-Z]+) to ([a-zA-Z]+) = ([0-9]+))");
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    Distance distance;
    distance.from = match[1];
    distance.to = match[2];
    distance.value = std::stoi(match[3]);
    distances.push_back(distance);    
  }
  std::vector<std::string> cities;
  for (const auto& d : distances) {
    cities.push_back(d.from);
    cities.push_back(d.to);
  }
  std::sort(std::begin(cities), std::end(cities));
  cities.erase(std::unique(cities.begin(), cities.end()), cities.end());
  std::vector<std::vector<int>> am(cities.size(), std::vector<int>(cities.size(), std::numeric_limits<int>::min()));
  for (const auto& d : distances) {
    const int i1 = std::distance(cities.begin(), std::find(cities.begin(), cities.end(), d.from));
    const int i2 = std::distance(cities.begin(), std::find(cities.begin(), cities.end(), d.to));
    am[i1][i2] = d.value;
    am[i2][i1] = d.value;
  }
  std::cout << find_longest_path(cities, am) << '\n';
  return 0;
}
