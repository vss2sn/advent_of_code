#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool is_number (const char c) {
  return c >= '0' && c <= '9';
}

void debug_print(const std::string& packet1, const std::string& packet2, const int i1, const int i2) {
  std::cout << packet1 << ' ' << packet2 << '\n';
  for (int i = 0; i < i1; i++) std::cout << ' ';
  std::cout << '^';
  for (int i = i1 + 1; i < packet1.size(); i++) std::cout << ' ';
  std::cout << ' ';
  for (int i = 0; i < i2; i++) std::cout << ' ';
  std::cout << '^';
  for (int i = i2 + 1; i < packet2.size(); i++) std::cout << ' ';
  std::cout << '\n';
}

bool compare (const std::string& packet1, const std::string& packet2) {
  int i1 = 0;
  int i2 = 0;
  while (i1 < packet1.size() && i2 < packet2.size()) {
    if (is_number(packet1[i1]) && is_number(packet2[i2])) {
      int n1 = 0;
      while (is_number(packet1[i1])) {
        n1 = n1 * 10 + (packet1[i1] - '0');
        i1++;
      }
      int n2 = 0;
      while (is_number(packet2[i2])) {
        n2 = n2 * 10 + (packet2[i2] - '0');
        i2++;
      }
      if (n1 == n2) {
        // debug_print(packet1, packet2, i1, i2);
        // std::cout << "Equal" << '\n';
        continue;
      }
      if (n1 < n2) {
        // debug_print(packet1, packet2, i1, i2);
        // std::cout << "Correct order" << '\n';
        return true;
      }
      break;
    } else if (packet1[i1] == packet2[i2]) {
      i1++;
      i2++;
    } else if (packet1[i1] == ']') {
      // debug_print(packet1, packet2, i1, i2);
      // std::cout << "Correct order" << '\n';
      return true;
    } else if (packet2[i2] == ']') {
      break;
    } else if (packet1[i1] == '[' || packet1[i1] == ',') {
      i1++;
    } else if (packet2[i2] == ',' || packet2[i2] == '[') {
      i2++;
    }
  }
  if (i1 == packet1.size()) {
    // debug_print(packet1, packet2, i1, i2);
    // std::cout << "Correct order" << '\n';
    return true;
  }
  // debug_print(packet1, packet2, i1, i2);
  // std::cout << "Wrong order" << '\n';
  return false;
}


int main(int argc, char * argv[]) {
  std::string input = "../input/day_13_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> packets;
  while(std::getline(file, line)) {
    if (line.empty()) continue;
    packets.push_back(line);
  }

  const std::vector<std::string> divider_packets {
    "[[2]]",
    "[[6]]"
  };

  for (const auto& divider_packet : divider_packets) {
    packets.push_back(divider_packet);
  }

  std::sort(std::begin(packets), std::end(packets), compare);

  int ans = 1;
  for (const auto& divider_packet : divider_packets) {
    ans *= (std::distance(std::begin(packets), std::find(std::begin(packets), std::end(packets), divider_packet)) + 1);
  }

  std::cout << ans << '\n';
  return 0;
}
