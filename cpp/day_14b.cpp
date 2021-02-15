#include <bitset>
#include <numeric>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <unordered_map>

std::string ParseMaskLine(const std::string& line) {
  const std::regex mask_pattern(R"(mask = ([10X]+))");
  std::smatch mask_match;
  std::regex_search(line, mask_match, mask_pattern);
  return mask_match[1];
}

std::pair<uint64_t, uint64_t> ParseMemLine(const std::string& line) {
  const std::regex mem_pattern(R"(mem\[(\d+)\] = (\d+))");
  std::smatch mem_match;
  std::regex_search(line, mem_match, mem_pattern);
  return {std::stoul(mem_match[1]), std::stoul(mem_match[2])};
}

std::vector<uint64_t> GenerateAddresses(const std::string& mask, std::bitset<36> address) {
  std::vector<uint64_t> floating_bits;
  for(int i = 0; i < mask.size(); ++i) {
    if(mask[i] == 'X') {
      floating_bits.push_back(mask.size() - i - 1);
    } else if (mask[i] == '1') {
      address[mask.size() - i - 1] = true;
    }
  }

  uint64_t address_count = 1u << floating_bits.size();
  std::vector<uint64_t> addresses;
  addresses.reserve(address_count);
  for (uint64_t bits = 0; bits < address_count; ++bits) {
    std::bitset<36> new_address(address);
    for(uint64_t i = 0; i < floating_bits.size(); ++i) {
      new_address[floating_bits[i]] = (bits & (1u << i)) != 0;
    }
    addresses.push_back(new_address.to_ullong());
  }
  return addresses;
}

int main() {
  std::fstream file{"day_14_data.txt"};
  std::string line;
  std::string mask;
  std::unordered_map<uint64_t, uint64_t> memory;
  while (std::getline(file, line)) {
    if(line.substr(0, 4) == "mask") {
      mask = ParseMaskLine(line);
    } else if (line.substr(0, 3) == "mem") {
      const auto [address, value] = ParseMemLine(line);
      std::vector<uint64_t> addresses = GenerateAddresses(mask, address);
      for (const auto& addr : addresses) {
        memory[addr] = value;
      }
    } else {
      std::cout << "This should not happen" << '\n';
    }
  }
  auto acc_func = [](const uint64_t sum, const auto& ele) {
    return sum + ele.second;
  };

  const uint64_t sum = std::accumulate(std::begin(memory), std::end(memory), (uint64_t)0, acc_func);
  std::cout << sum << '\n';
  return sum;
}
