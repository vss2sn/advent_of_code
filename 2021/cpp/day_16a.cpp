#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Packet;

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

Packet convert_to_packet(const std::string& s, int& index) {
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
      return p ;
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

long long sum_packet_version_numbers(const Packet& p) {
  long long p_val = p.version;
  if (p.value == -1) {
    for (const auto& sub_p : p.sub_ps) {
      p_val += sum_packet_version_numbers(sub_p);
    }
  }
  return p_val;
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
  int index = 0;
  auto p = convert_to_packet(b, index);
  std::cout << sum_packet_version_numbers(p) << '\n';
  return 0;
}
