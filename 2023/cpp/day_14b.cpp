#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr long long iterations = 1000000000;

// Can refactor to use a single function for motion 
// Or can rotate the platform and use only the north function

void tilt_east(std::vector<std::string>& platform) {
  for (int col_idx = platform[0].size() - 1; col_idx >= 0; col_idx--) {
    for (int row_idx = 0; row_idx < platform.size(); row_idx++) {
      if (platform[row_idx][col_idx] == 'O') {
        int new_col_idx = col_idx + 1;
        while (new_col_idx < platform[0].size()) {
          if (platform[row_idx][new_col_idx] != '.') {
            new_col_idx--;
            break;
          }
          new_col_idx++;
        }
        if (new_col_idx >= platform.size()) {
          new_col_idx = platform.size() - 1;
        }
        if (col_idx != new_col_idx) {
          platform[row_idx][col_idx] = '.';
          platform[row_idx][new_col_idx] = 'O';
        }
      }
    }
  }
}

void tilt_west(std::vector<std::string>& platform) {
  for (int col_idx = 1; col_idx < platform[0].size(); col_idx++) {
    for (int row_idx = 0; row_idx < platform.size(); row_idx++) {
      if (platform[row_idx][col_idx] == 'O') {
        int new_col_idx = col_idx - 1;
        while (new_col_idx >= 0) {
          if (platform[row_idx][new_col_idx] != '.') {
            new_col_idx++;
            break;
          }
          new_col_idx--;
        }
        if (new_col_idx < 0) {
          new_col_idx = 0;
        }
        if (col_idx != new_col_idx) {
          platform[row_idx][col_idx] = '.';
          platform[row_idx][new_col_idx] = 'O';
        }
      }
    }
  }
}

void tilt_south(std::vector<std::string>& platform) {
  for (int row_idx = platform.size() - 1; row_idx >= 0; row_idx--) {
    for (int col_idx = 0; col_idx < platform[0].size(); col_idx++) {
      if (platform[row_idx][col_idx] == 'O') {
        int new_row_idx = row_idx + 1;
        while (new_row_idx < platform.size()) {
          if (platform[new_row_idx][col_idx] != '.') {
            new_row_idx--;
            break;
          }
          new_row_idx++;
        }
        if (new_row_idx >= platform.size()) {
          new_row_idx = platform.size() - 1;
        }
        if (row_idx != new_row_idx) {
          platform[row_idx][col_idx] = '.';
          platform[new_row_idx][col_idx] = 'O';
        }
      }
    }
  }
}


void tilt_north(std::vector<std::string>& platform) {
  for (int row_idx = 1; row_idx < platform.size(); row_idx++) {
    for (int col_idx = 0; col_idx < platform[0].size(); col_idx++) {
      if (platform[row_idx][col_idx] == 'O') {
        int new_row_idx = row_idx - 1;
        while (new_row_idx >= 0) {
          if (platform[new_row_idx][col_idx] != '.') {
            new_row_idx++;
            break;
          }
          new_row_idx--;
        }
        if (new_row_idx < 0) {
          new_row_idx  = 0;
        }
        if (row_idx != new_row_idx) {
          platform[row_idx][col_idx] = '.';
          platform[new_row_idx][col_idx] = 'O';
        }
      }
    }
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> platform;
  while(std::getline(file, line)) {
    platform.push_back(line);
  }


  std::vector<std::vector<std::string>> cache;
  const auto check_seen_before = [&cache, & platform]() {
    for (int idx = 0; idx < cache.size(); idx++) {
      bool equal = true;
      for (int i = 0; i < platform.size() && equal; i++) {
        if (platform[i] != cache[idx][i]) {
          equal = false;
        }
      }
      if (equal) return idx;
    }
    return -1;
  };

  int first_seen_at = -1;
  int seen_again_at = -1; // kept for debug
  bool first = true;
  for (long long i = 0; i < iterations; i++) {
    if (const auto idx = check_seen_before(); idx != -1) {
      first_seen_at = idx;
      // std::cout << "Found at " << first_seen_at << ", repeated at " << i << '\n';
      seen_again_at = i;
      break;
    } else {
      cache.push_back(platform);
    }
    tilt_north(platform);
    tilt_west(platform);
    tilt_south(platform);
    tilt_east(platform);
  }

  // Calculate load
  const int delta = seen_again_at - first_seen_at;
  const int idx = (iterations - first_seen_at) % (delta) + first_seen_at;
  const auto& final_platform = cache[idx];
  int ans = 0;
  for (int row_idx = 0; row_idx < final_platform.size(); row_idx++) {
    for (int col_idx = 0; col_idx < final_platform[0].size(); col_idx++) {
      ans += (final_platform[row_idx][col_idx] == 'O' ? final_platform.size() - row_idx : 0);
    }
  }
  std::cout << ans << '\n';

  return 0;
}