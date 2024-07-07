#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <string_view>
#include <vector>

struct Disc {
    long id;
    long position;
    long n_positions;
};

long sv_to_long (const std::string_view sv) {
  bool negative = (sv[0] == '-') ? true : false;
  long n = 0;
  for (long i = negative ? 1 : 0; i < sv.size(); i++) {
    n = n * 10 + (sv[i] - '0');
  }
  if (negative) {
    n *= -1;
  }
  return n;
}

std::tuple<long, long, long> extended_gcd(long a, long b) {
    auto r = std::pair<long, long>(a, b);
    auto s = std::pair<long, long>(1, 0);
    auto t = std::pair<long, long>(0, 1);
    while (r.second != 0) {
        const long quotient = r.first / r.second;
        r = std::pair<long, long>(r.second, (r.first - (quotient * r.second) + r.second) % r.second);
        s = std::pair<long, long>(s.second, s.first - (quotient * s.second));
        t = std::pair<long, long>(t.second, t.first - (quotient * t.second));
    }
    return {r.first, s.first, t.first};
}

std::pair<long, long> lcm_with_phase_difference (const long a_period, const long b_period, const long a_phase , const long b_phase) {
    const auto [gcd, s, t] = extended_gcd(a_period, b_period);
    const long phase_difference = a_phase - b_phase;
    const long pd_mult = phase_difference / gcd;
    const long pd_remainder = phase_difference % gcd; // should be 0
    const long combined_period = (a_period / gcd) * b_period;
    long combined_phase = (a_phase - s * pd_mult * a_period) % combined_period;
    while(combined_phase < 0) combined_phase += combined_period;
    combined_phase %= combined_period;
    return {combined_period, combined_phase};
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  std::vector<Disc> discs;
  while(std::getline(file, line)) {
    std::vector<std::string_view> split_line;
    std::ranges::copy(line 
                      | std::ranges::views::split(' ') 
                      | std::ranges::views::transform([](auto&& rng) {return std::string_view(rng);}), 
                            std::back_inserter(split_line));
    Disc disc;
    disc.id = sv_to_long(split_line[1].substr(1, split_line[1].size() - 1));
    disc.n_positions = sv_to_long(split_line[3]);
    disc.position = sv_to_long(split_line[11].substr(0, split_line[11].size() - 1));
    disc.position = disc.position % disc.n_positions;
    discs.push_back(disc);
  } 
  {
    Disc disc;
    disc.id = discs.size() + 1;
    disc.position = 0;
    disc.n_positions = 11;
    discs.push_back(disc);
  }
  std::pair<long, long> lcm = {1, 0};
  for (const auto& disc : discs) {
    // Calculate using offset to ensure that the disc is in line when the capsule hits
    lcm = lcm_with_phase_difference(lcm.first, disc.n_positions, lcm.second, disc.n_positions - (disc.position + disc.id) % disc.n_positions);
  }
  std::cout << (lcm.second + lcm.first) % lcm.first << '\n';
  return 0;
}