#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[]) {

  std::string input = "../input/day_04_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  const std::regex pattern(R"(\[([0-9]+)-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+)\] (.*))");
  std::vector<std::tuple<int, int, int, int, int, std::string>> data;
  std::string line;
  std::unordered_map<int, std::vector<int>> guard_sleep_records;

  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    if (std::string(match[6]).substr(0,5) == "Guard") {
      const auto s = std::string(match[6]);
      std::size_t start = s.find(' ', 0);
      std::size_t end = s.find(' ', start + 1);
      guard_sleep_records[stoi(s.substr(start+2,  end - start - 2))] = std::vector<int>(60, 0);
    }
    data.emplace_back(stoi(match[1]), stoi(match[2]), stoi(match[3]), stoi(match[4]), stoi(match[5]), match[6]);
  }
  std::sort(data.begin(), data.end());

  int guard = -1;
  int start = 0;
  int end = 0;
  for (const auto& [year, month, date, hour, minute, s] : data) {
    if (s.substr(0, 5) == "Guard") {
      std::size_t start = s.find(' ', 0);
      std::size_t end = s.find(' ', start + 1);
      guard = stoi(s.substr(start+2,  end - start - 2));
    } else if (s.substr(0, 5) == "falls"){
      start = minute;
    } else if (s.substr(0, 5) == "wakes"){
      end = minute;
      for (int i = start; i < end; i++) {
        guard_sleep_records[guard][i]++;
      }
    }
  }

  int id_of_guard_who_sleeps_most_freq = -1, n_times_of_sleep_at_most_freq_asleep_min = -1, minute_at_which_most_freq_asleep = -1;
  for (const auto& [id, sleep_record] : guard_sleep_records) {
    const auto most_freq_asleep_min_it = std::max_element(std::begin(sleep_record), std::end(sleep_record));
    if (*most_freq_asleep_min_it > n_times_of_sleep_at_most_freq_asleep_min) {
      id_of_guard_who_sleeps_most_freq = id;
      n_times_of_sleep_at_most_freq_asleep_min = *most_freq_asleep_min_it;
      minute_at_which_most_freq_asleep = most_freq_asleep_min_it - std::begin(guard_sleep_records[id]);
    }
  }
  const int ans = id_of_guard_who_sleeps_most_freq * minute_at_which_most_freq_asleep;
  std::cout << ans << '\n';
  return ans;
}
