#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::pair<long long, long long>> ParseBusFrequencies(
    const std::string& all_bus_f) {
  std::vector<std::pair<long long, long long>> freqs_is;  // frequency, index
  long long begin = 0;
  long long end = all_bus_f.find(",", begin);
  long long index = 0;
  while (end != std::string::npos) {
    const std::string bus_f = all_bus_f.substr(begin, end - begin);
    if (bus_f.find_first_not_of("1234567890") == std::string::npos) {
      freqs_is.push_back({stoi(bus_f), stoi(bus_f) - index});
    }
    ++index;
    begin = end + 1;
    end = all_bus_f.find(",", begin);
  }
  const std::string bus_f = all_bus_f.substr(begin, end - begin);
  if (bus_f.find_first_not_of("1234567890") == std::string::npos) {
    freqs_is.push_back({stoi(bus_f), stoi(bus_f) - index});
  }
  return freqs_is;
}

long long findGCD(const long long a, const long long b) {
  if (a == 0) {
    return b;
  } else {
    return findGCD(b % a, a);
  }
}

bool checkArrayCoPrime(const std::vector<std::pair<long long, long long>>& v) {
  long long prod = 1;
  for (const auto [ele, ign] : v) {  // element, ignore_element
    if (findGCD(ele, prod) != 1) {
      return false;
    }
    prod *= ele;
  }
  return true;
}

// Euclid extended algorithms
std::tuple<long long, long long, long long> extendedGCD(const long long a,
                                                        const long long b) {
  if (a == 0) {
    return {b, 0, 1};
  }
  const auto [gcd, prev_x, prev_y] = extendedGCD(b % a, a);
  return {gcd, prev_y - ((b / a) * prev_x), prev_x};
}

long long modInverse(const long long a, const long long m) {
  const auto [gcd, x, y] = extendedGCD(a, m);
  if (gcd == 1) {
    return (x % m + m) % m;
  } else {
    return -1;
  }
}

long long ChineseRemainderTheorem(
    const std::vector<std::pair<long long, long long>>
        divs_rems) {  // vector of <divisors, remainders>
  // calculate product
  long long prod = 1;
  for (const auto [div, rem] : divs_rems) {
    prod *= div;
  }

  long long result = 0;
  for (const auto [div, rem] : divs_rems) {
    const long long prodbyd = prod / div;  // prduct divided by div
    const auto [gcd, x, y] = extendedGCD(prodbyd, div);
    result = (prod + result + (rem * prodbyd * x)) % prod;
  }
  return result;
}

int main() {
  std::ifstream file{"../input/day_13_input"};

  long long arrive_abs;  // attive at bus stop
  file >> arrive_abs;

  std::string all_bus_f;  // all buses frequencies
  file >> all_bus_f;

  long long earliest_bus;             // To be found
  long long extra_time = arrive_abs;  // Ensures > arrive_abs % all_bus_f
  std::vector<std::pair<long long, long long>> freqs_is =
      ParseBusFrequencies(all_bus_f);  // frequencies, indices

  if (!checkArrayCoPrime(freqs_is)) {
    std::cout << "Frequencies not co prime, use brute force" << '\n';
    return -1;
  }

  // To compare with chinese remainder theorem
  // Find x such that:
  // x % freq = index
  const long long result = ChineseRemainderTheorem(freqs_is);
  std::cout << result << '\n';
  return result;
}
