#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <memory>
#include <numeric>

using namespace std::chrono_literals;

struct Node {
  std::vector<std::unique_ptr<Node>> children;
  std::vector<int> metadata;
};

std::unique_ptr<Node> createChild(const std::vector<int>& input, int& index) {
  // std::this_thread::sleep_for(1s);
  const int n_children = input[index];
  const int n_metadata = input[index+1];
  // std::cout << "index: " << index << '\n';
  // std::cout << "number of children : " << n_children << '\n';
  // std::cout << "number of metadata : " << n_metadata << '\n';

  index += 2;
  std::unique_ptr<Node> this_node = std::make_unique<Node>();
  for (int i = 0; i < n_children; i++) {
    // std::cout << "index of child: " << index << '\n';
    // std::cout << "number of childred of child: " << input[index] << '\n';
    this_node->children.push_back(createChild(input, index));
  }
  for (int i = 0; i < n_metadata; i++) {
    // std::cout << "index of metadata: " << index << '\n';
    // std::cout << "value of metadata: " << input[index] << '\n';
    this_node->metadata.push_back(input[index]);
    index++;
  }
  return this_node;
}

std::unique_ptr<Node> parseTree(const std::string& input) {
  size_t start = 0;
  const std::string delimiter = " ";
  size_t end = input.find(delimiter);
  std::vector<int> parsed_input;
  while (end < std::string::npos) {
    parsed_input.emplace_back(stoi(input.substr(start, end - start)));
    start = end + delimiter.size();
    end = input.find(delimiter, start);
  }
  parsed_input.emplace_back(stoi(input.substr(start, input.size() - start)));
  int index = 0;
  return createChild(parsed_input, index);
}

int sumMetadata(const Node* node) {
  int sum = 0;
  for (const auto& c : node->children) {
    sum += sumMetadata(c.get());
  }
  sum += std::accumulate(node->metadata.begin(), node->metadata.end(), 0);
  return sum;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  auto root = parseTree(line);
  const int ans = sumMetadata(root.get());
  std::cout << ans << '\n';
  return ans;
}
