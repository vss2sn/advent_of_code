#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>

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
  // std::cout << mem_match[1] << ' '<< mem_match[2] << '\n';
  return {std::stoul(mem_match[1]), std::stoul(mem_match[2])};
}

std::pair<uint64_t, uint64_t> SplitMask (const std::string& mask) {
  std::string set_mask = mask;
  std::string clear_mask = mask;
  for (int i = 0; i < mask.size(); ++i) {
    if (mask[i] == 'X') {
      clear_mask[i] = '1';
    } else {
      clear_mask[i] = '0';
    }
    if(mask[i] == '1') {
      set_mask[i] = '1';
    } else {
      set_mask[i] = '0';
    }
  }
  return std::make_pair(std::stoul(clear_mask, 0, 2), std::stoul(set_mask, 0, 2));
}

int main() {
  std::fstream file{"day_14_data.txt"};
  std::string line;
  std::unordered_map<uint64_t, uint64_t> memory;
  uint64_t clear_mask = 0;
  uint64_t set_mask = 0;
  while (std::getline(file, line)) {
    if(line.substr(0, 4) == "mask") {
      std::tie(clear_mask, set_mask) = SplitMask( ParseMaskLine(line) );
    } else if (line.substr(0, 3) == "mem") {
      const auto [address, value] = ParseMemLine(line);
      memory[address] = (value & clear_mask) | set_mask;
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
