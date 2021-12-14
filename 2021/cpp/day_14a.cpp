#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Pair {
  char c1, c2;
  Pair(const char c1, const char c2) : c1(c1), c2(c2) {}
  bool operator == (const Pair& p) const {
    return c1 == p.c1 && c2 == p.c2;
  }
  friend std::ostream& operator << (std::ostream& os, const Pair& p);
};

std::ostream& operator << (std::ostream& os, const Pair& p) {
  os << "(" << p.c1 << ", " << p.c2 << ")";
  return os;
}

struct hash_pair {
  std::size_t operator () (const Pair& p) const {
    return p.c1 << 5 + p.c2;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::unordered_map<Pair, long long, hash_pair> pairs;
  std::getline(file, line);
  const auto start_letter = line[0];
  const auto end_letter = line[line.size() -1];
  // Using [] to default construct 0 if the element doesn't exist
  for (size_t i = 0; i < line.size() - 1; i++) {
    pairs[Pair(line[i], line[i+1])]++;
  }

  std::unordered_map<Pair, char, hash_pair> rules;
  while(std::getline(file, line)) {
    rules[Pair(line[0], line[1])] = line[6];
  }

  constexpr unsigned long long n = 10;
  for (size_t i = 0; i < n; i++) {
    std::unordered_map<Pair, long long, hash_pair> pairs_new;
    for (const auto& [p, count] : pairs) {
      if (const auto it = rules.find(p); it != rules.end()) {
        pairs_new[Pair(p.c1, it->second)] += count;
        pairs_new[Pair(it->second, p.c2)] += count;
      } else {
        pairs_new[Pair(p.c1, p.c2)] += count;
      }
    }
    pairs = pairs_new;
  }

  std::unordered_map<char, long long> m;
  for (const auto& [p, count] : pairs) {
    m[p.c1] += count;
    m[p.c2] += count;
  }

  m[start_letter]++;
  m[end_letter]++;

  auto max_count = std::numeric_limits<long long>::min();
  auto min_count = std::numeric_limits<long long>::max();

  for (const auto& p : m) {
    max_count = std::max(max_count, p.second);
    min_count = std::min(min_count, p.second);
  }
  std::cout << (max_count - min_count)/2 << '\n';

  return 0;
}
