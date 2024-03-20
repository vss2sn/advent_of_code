#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>  
#include <unordered_map>  
#include <random>
#include <cassert>
#include <stack>

// Used Karger's algorithm
// Refactor this; should run much faster
// Also, other algos are available

struct Edge {
  std::string start;
  std::string end;
  std::size_t hash = 0;
  bool operator == (const Edge& e) const{
    return start == e.start && end == e.end;
  }
  void update_hash() {
    std::size_t p = 1;
    for (int i = 0; i < start.size(); i++) {
      hash += p * (start[i] - 'a');
      p*= 10;
    }
    for (int i = 0; i < end.size(); i++) {
      hash += p * (start[i] - 'a');
      p *= 10;
    }
  }
};

struct hasher {
  std::size_t operator () (const Edge& e) const {
    return e.hash;

  }
};

void parse_input(const std::string& input_str, std::unordered_set<Edge, hasher>& edges) {
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
    edge.update_hash();
    edges.insert(edge);
  }
  const std::string to = input_str.substr(start, input_str.size() - start);
  Edge edge;
  edge.start = from;
  edge.end = to;
  edges.insert(edge);
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

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1000);

void KargersAlgorithm(std::vector<Edge> edges, 
                      std::size_t n_vertices,
                      std::unordered_map<std::string, std::pair<std::string, int>> subsets,
                      std::unordered_map<Edge, int, hasher>& answers) {
  int count = edges.size();
  while (n_vertices > 2) {
    const auto idx = dist(rng) % (count);
    const auto& edge = edges[idx];
    const auto& p1 = UnionFindRCFindUtil(edge.start, subsets);
    const auto& p2 = UnionFindRCFindUtil(edge.end, subsets);
    if (p1 != p2) {
      UnionFindRCUnionUtil(p1, p2, subsets);
      n_vertices--;
    }
    count--;
    std::swap(edges[idx], edges[count]);
  }
  for (int i = 0; i < count; i++) {
    const auto& p1 = UnionFindRCFindUtil(edges[i].start, subsets);
    const auto& p2 = UnionFindRCFindUtil(edges[i].end, subsets);
    if (p1 != p2) {
      answers[edges[i]]++;
    }
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_25_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::unordered_set<Edge, hasher> edges_main;
  while(std::getline(file, line)) {
    parse_input(line, edges_main);
  }

  dist = std::uniform_int_distribution<std::mt19937::result_type>(edges_main.size());
  std::unordered_set<std::string> vertices;
  std::vector<Edge> edges_v;
  for (const auto& edge : edges_main) {
    vertices.insert(edge.start);
    vertices.insert(edge.end);
    edges_v.push_back(edge);
  }
  
  std::unordered_map<Edge, int, hasher> answers;

  std::unordered_map<std::string, std::pair<std::string, int>> subsets;
  for (const auto& v : vertices) {
    subsets[v] = {v, 0};
  }
  const auto n_vertices = vertices.size();
  for (int iteration = 0; iteration < 10000; iteration++) {
    if (iteration % 100 == 0) std::cout << "Iteration: " << iteration << '\n';
    KargersAlgorithm(edges_v, n_vertices, subsets, answers);
  }
  // Create the edge list with the top 3 edges removed.
  auto edges = edges_main;
  { 
    std::vector<std::pair<Edge, int>> answers_v;
    for (const auto& [edge, count] : answers) {
      answers_v.push_back({edge, count});
    }
    std::partial_sort(std::begin(answers_v), std::begin(answers_v) + 3, std::end(answers_v), [](const auto& e1, const auto& e2) { return e1.second > e2.second; });
    edges.erase(answers_v[0].first);
    edges.erase(answers_v[1].first);
    edges.erase(answers_v[2].first);
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
