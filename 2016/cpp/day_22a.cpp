#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <regex>
#include <vector>  

struct Node {
    std::string name;
    int x;
    int y;
    int size;
    int used;
    int avail;
    int p_used;
    
};

int sv_to_int (const std::string_view sv) {
  bool negative = (sv[0] == '-') ? true : false;
  int n = 0;
  for (int i = negative ? 1 : 0; i < sv.size(); i++) {
    n = n * 10 + (sv[i] - '0');
  }
  if (negative) {
    n *= -1;
  }
  return n;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  std::getline(file, line);
  std::vector<Node> nodes;
  const std::regex pattern(R"((/dev/grid/node-x([0-9]+)-y([0-9]+))[ ]+([0-9]+)T[ ]+([0-9]+)T[ ]+([0-9]+)T[ ]+([0-9]+)%)");
  while(std::getline(file, line)) {
    std::cout << line << '\n';
    std::smatch match;
    std::regex_search(line, match, pattern);
    for(const auto ele : match) {
      std::cout << ele << ' ';
    }
    std::cout << '\n';
    Node node;
    node.name = match[1];
    node.x = std::stoi(match[2]);
    node.y = std::stoi(match[3]);
    node.size = std::stoi(match[4]);
    node.used = std::stoi(match[5]);
    node.avail = std::stoi(match[6]);
    node.p_used = std::stoi(match[7]);
    nodes.push_back(node);
  }
  int count = 0;
  for (int idx_1 = 0; idx_1 < nodes.size(); idx_1++) {
    if (nodes[idx_1].used == 0) continue;
    for (int idx_2 = 0; idx_2 < nodes.size(); idx_2++) {
      if (idx_2 == idx_1) continue;
      if (nodes[idx_2].avail >= nodes[idx_1].used) {
        count++;
      }
    }  
  }
  std::cout << count << '\n';
  return 0;
}