#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

struct NanoBot {
  long long x, y, z, r;

  NanoBot (const long long x, const long long y, const long long z, const long long r) : x(x), y(y), z(z), r(r) {}
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_23_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  const std::regex pattern(R"(<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, r=([0-9]+))");
  std::vector<NanoBot> bots;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    bots.emplace_back(std::stoll(match[1]), std::stoll(match[2]), std::stoll(match[3]), std::stoll(match[4]));
  }

  std::sort(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) { return b1.r > b2.r; });
  const auto ans = std::count_if (
    std::begin(bots),
    std::end(bots),
    [&bot_main=std::as_const(bots[0])](const auto& bot) {
      return std::abs(bot.x-bot_main.x) + std::abs(bot.y-bot_main.y) + std::abs(bot.z-bot_main.z) <= bot_main.r;
    }
  );

  std::cout << ans << '\n';
  return ans;
}
