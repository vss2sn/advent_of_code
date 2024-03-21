#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <stack>
#include <unordered_set>  
#include <unordered_map>  
#include <vector>

// Used Karger's algorithm
// Also, other algorithms are available

struct Edge {
  std::string start;
  std::string end;
  bool operator == (const Edge& e) const{
    return start == e.start && end == e.end;
  }
};

void parse_input(const std::string& input_str, std::vector<Edge>& edges) {
  const std::string from = input_str.substr(0, 3);
  std::size_t start = 5;
  std::size_t end = input_str.find(' ', start);
  while (end != std::string::npos) {
    const std::string to = input_str.substr(start, end - start);
    start = end + 1;
    end = input_str.find(' ', start);
    Edge edge;
    edge.start = from;
    edge.end = to;
    edges.push_back(edge);
  }
  const std::string to = input_str.substr(start, input_str.size() - start);
  Edge edge;
  edge.start = from;
  edge.end = to;
  edges.push_back(edge);
}

const std::string& UnionFindRCFindUtil(const std::string& v, std::unordered_map<std::string, std::pair<std::string, int>>& subsets){
  if(v != subsets[v].first) {
    subsets[v].first = UnionFindRCFindUtil(subsets[v].first, subsets);
  }
  return subsets[v].first;
}

void UnionFindRCUnionUtil(const std::string& v1, const std::string& v2, std::unordered_map<std::string, std::pair<std::string, int>>& subsets) {
  const auto& p1 = UnionFindRCFindUtil(v1, subsets);
  const auto& p2 = UnionFindRCFindUtil(v2, subsets);

  if(subsets[p1].second > subsets[p2].second) {
    subsets[p2].first = p1;
  } else if(subsets[p1].second < subsets[p1].second) {
    subsets[p1].first = p2;
  } else {
    subsets[p2].first = p1;
    subsets[p1].second++;
  }
}

bool KargersAlgorithm(std::vector<Edge>& edges, 
                      std::size_t n_vertices,
                      std::unordered_map<std::string, std::pair<std::string, int>> subsets,
                      std::vector<Edge>& wires_to_cut) {
  // Shuffle the order of edges here and then proceed in order instead of randomly choosing edges 
  static std::random_device dev;
  static std::mt19937 rng(dev());
  std::shuffle(edges.begin(), edges.end(), rng);

  int count = edges.size()-1;
  while (n_vertices > 2) {
    const auto& edge = edges[count];
    const auto& p1 = UnionFindRCFindUtil(edge.start, subsets);
    const auto& p2 = UnionFindRCFindUtil(edge.end, subsets);
    count--;
    if (p1 != p2) {
      UnionFindRCUnionUtil(p1, p2, subsets);
      n_vertices--;
    }
  }
  int count_without_self_edges = 0;
  for (int i = 0; i <= count; i++) {
    const auto& p1 = UnionFindRCFindUtil(edges[i].start, subsets);
    const auto& p2 = UnionFindRCFindUtil(edges[i].end, subsets);
    if (p1 != p2) {
      count_without_self_edges++;
      wires_to_cut.push_back(edges[i]);
    }
  }
  
  // Early breaking condition
  if (count_without_self_edges == 3) {
    return true;
  }
  wires_to_cut.clear();
  return false;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_25_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Edge> edges;
  while(std::getline(file, line)) {
    parse_input(line, edges);
  }

  std::unordered_set<std::string> vertices;
  for (const auto& edge : edges) {
    vertices.insert(edge.start);
    vertices.insert(edge.end);
  }
  
  std::vector<Edge> wires_to_cut;
  std::unordered_map<std::string, std::pair<std::string, int>> subsets;
  for (const auto& v : vertices) {
    subsets[v] = {v, 0};
  }
  const auto n_vertices = vertices.size();
  // int iteration = 0;
  while (!KargersAlgorithm(edges, n_vertices, subsets, wires_to_cut)) {
    // std::cout << "Iteration: " << iteration << '\n';
    // iteration++;
  }
  
  // Remove the 3 wires to cut from the list of edges by 
  // moving them to the end of the vector and then erasing
  {
    int count = 0;
    for (const auto& wire : wires_to_cut) {
      auto it = std::find(std::begin(edges), std::end(edges), wire);
      auto end_it = edges.end();
      std::advance(end_it, -(count + 1));
      std::iter_swap(it, end_it);
      count++;
    }
    auto end_it = edges.end();
    std::advance(end_it, -count); 
    edges.erase(end_it, std::end(edges));
  }
  
  // Find all the points connecting to a single (random) point to find one of the disjoint sets
  std::unordered_map<std::string, std::unordered_set<std::string>> adj_list;
  for (const auto& e : edges) {
    adj_list[e.start].insert(e.end);
    adj_list[e.end].insert(e.start);
  }
  std::stack<std::string> stack;
  stack.push(adj_list.begin()->first);
  std::unordered_set<std::string> seen;
  while (!stack.empty()) {
    const auto current = stack.top();
    stack.pop();
    if (seen.find(current) != seen.end()) {
      continue;
    }
    seen.insert(current);
    for (const auto& ele : adj_list[current]) {
      if (seen.find(ele) == seen.end()) {
        stack.push(ele);
      }
    }
  }
  std::cout << seen.size()  * (vertices.size() - seen.size()) << '\n';
  return 0;
}
