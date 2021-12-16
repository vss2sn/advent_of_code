#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

struct Packet {
  int version;
  int type;
  long long value; // when type != 4, value is set to -1, as contains subpackets
  std::vector<Packet> sub_ps; // when type == 4, there will be no subpackets
  Packet (const int version, const int type, const long long value, const std::vector<Packet>& sub_ps) :
    version(version), type(type), value(value), sub_ps(sub_ps) {};
};

std::string convert_to_binary(const std::string& s) {
  std::unordered_map<char, std::string> m {
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"}
  };

  std::string r;
  for (const char c : s) {
    r += m[c];
  }
  return r;
}

long long convert_to_decimal(const std::string& s) {
  long long i = 0;
  for (const char c : s) {
    i = i * 2 + (c - '0');
  }
  return i;
}

long long calculate_packet_value(Packet& p) {
  if (p.value == -1) { // when type != 4, value is set to -1, as contains subpackets
    for (auto& sub_p : p.sub_ps) { // update the values of each subpacket
      calculate_packet_value(sub_p);
    }
    if (p.type == 0) {
      p.value = std::accumulate(std::begin(p.sub_ps), std::end(p.sub_ps), 0ll, [](const auto sum, const auto& sub_p) { return sum + sub_p.value; });
    } else if (p.type == 1) {
      p.value = std::accumulate(std::begin(p.sub_ps), std::end(p.sub_ps), 1ll, [](const auto sum, const auto& sub_p) { return sum * sub_p.value; });
    } else if (p.type == 2) {
      p.value = std::min_element(std::begin(p.sub_ps), std::end(p.sub_ps), [](const auto& p1, const auto& p2) { return p1.value < p2.value; })->value;
    } else if (p.type == 3) {
      p.value = std::max_element(std::begin(p.sub_ps), std::end(p.sub_ps), [](const auto& p1, const auto& p2) { return p1.value < p2.value; })->value;
    } else if (p.type == 5) {
      p.value = p.sub_ps[0].value > p.sub_ps[1].value ? 1 : 0;
    } else if (p.type == 6) {
      p.value = p.sub_ps[0].value < p.sub_ps[1].value ? 1 : 0;
    } else if (p.type == 7) {
      p.value = p.sub_ps[0].value == p.sub_ps[1].value ? 1 : 0;
    }
  }
  return p.value;
}

Packet convert_to_packet(const std::string& s, size_t& index) {
  const int version = convert_to_decimal(s.substr(index, 3));
  index += 3;
  const int type = convert_to_decimal(s.substr(index, 3));
  index += 3;
  if (type == 4) {
    std::string bin_num;
    while (index < s.size()) {
      if (s[index] == '1') {
        bin_num += s.substr(index + 1, 4);
        index += 5;
      } else if (s[index] == '0') {
        bin_num += s.substr(index + 1, 4);
        index += 5;
        break;
      }
    }
    return Packet(version, type, convert_to_decimal(bin_num), std::vector<Packet>());
  } else {
    const int length_id = convert_to_decimal(s.substr(index, 1));
    index += 1;
    if (length_id == 0) {
      const auto l_bits  = convert_to_decimal(s.substr(index, 15));
      index += 15;
      const auto init_index = index;
      auto p = Packet(version, type, -1, std::vector<Packet>());
      while (index < init_index + l_bits) {
        p.sub_ps.emplace_back(convert_to_packet(s, index));
      }
      return p;
    } else if (length_id == 1) {
      const auto n_sub_ps  = convert_to_decimal(s.substr(index, 11));
      index += 11;
      auto p = Packet(version, type, -1, std::vector<Packet>());
      for (int i = 0; i < n_sub_ps; i++) {
        p.sub_ps.emplace_back(convert_to_packet(s, index));
      }
      return p;
    }
  }
  return Packet(0, 0 , 0, {});
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::vector<int>> map;
  std::getline(file, line);

  const auto b = convert_to_binary(line);
  size_t index = 0;
  auto p = convert_to_packet(b, index);
  std::cout << calculate_packet_value(p) << '\n';
  return 0;
}
