#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// Updated to use the method implemented in day_12b.cpp

struct Key {
  int reconstructed_size;
  int duplicated_idxdx;

  Key(const int reconstructed_size,
      const int duplicated_idxdx) 
      : reconstructed_size(reconstructed_size), duplicated_idxdx(duplicated_idxdx) {}

  bool operator == (const Key& k) const {
    return reconstructed_size == k.reconstructed_size && duplicated_idxdx == k.duplicated_idxdx;
  }
};

struct hasher {
  std::size_t operator () (const Key& p) const {
    return p.reconstructed_size * p.duplicated_idxdx;
  }
};

struct Record {
  std::string damaged; // damaged record
  std::vector<int> duplicated; // duplicated record
};

bool compare(const std::string& damaged, const std::vector<char>& reconstructed) {
  for (int i = 0; i < reconstructed.size(); i++) {
    if (damaged[i] != '?' && reconstructed[i] != damaged[i]) {
      return false;
    }
  }
  return true;
}

std::vector<int> extract_numbers(const std::string& s) {
  // std::cout << '|' << s << '|' << '\n';
  std::vector<int> numbers;
  std::size_t start = 0;
  std::size_t end = s.find(',', start);
  while (end != std::string::npos) {
    numbers.push_back(std::stoi(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(',', start);
  }
  numbers.push_back(std::stoi(s.substr(start, s.size() - start)));
  return numbers;
}

// Note that the last spring in reconstructed is always a '.'
bool compare_with_duplicated(const std::vector<char>& reconstructed, const std::vector<int>& duplicated) {
  int idx_duplicated = 0;
  int idx_reconstructed = 0;
  while (idx_reconstructed < reconstructed.size()) {
    if (reconstructed[idx_reconstructed] == '#') {
      for (int j = 0; j < duplicated[idx_duplicated]; j++) {
        if (reconstructed[idx_reconstructed] != '#') return false;
        idx_reconstructed++;
      }
      idx_duplicated++;
      if (reconstructed[idx_reconstructed] != '.') return false;
    }
    idx_reconstructed++;
  }
  return true;
}


void dp(const std::string& damaged, std::vector<char>& reconstructed, const std::vector<int>& duplicated, std::size_t& count, const int duplicated_idx, std::unordered_map<Key, std::size_t, hasher>& seen) {
  // Does not match pattern in damaged record (note compare matches upto the length of reconstructed only)
  if (!compare(damaged, reconstructed)) {
    return;
  }

  // Reconstructed pattern does not duplicate record
  if (!compare_with_duplicated(reconstructed, duplicated)) {
    return ;
  }

  // If the reconstructed record and the damage record ahve the same size and are equal
  if (damaged.size() == reconstructed.size() && compare(damaged, reconstructed)) {
    // If the number of damaged groups is correct
    if (duplicated_idx == duplicated.size()) {
      count++;
    }
    return;
  }

  // number of damaged + 1 * (number of intervals between each damaged group) > number of springs remaining 
  // 1 is the minimm interval between each damaged group
  if (std::begin(duplicated) + duplicated_idx <= std::end(duplicated) && ((std::accumulate(std::begin(duplicated) + duplicated_idx, std::end(duplicated), 0) + duplicated.size() - duplicated_idx) > (damaged.size() - reconstructed.size())) ) {
    return;
  }
  
  const std::size_t count_on_entry = count; // count on entry into dp function
  // Util function for checking cache before calling the dp function
  const auto& dp_wrapper_with_cache_check = [&damaged, &reconstructed, &duplicated, &count, &seen] (const int duplicated_idx) {
    if (const auto it = seen.find(Key(reconstructed.size(), duplicated_idx)); it != seen.end()) {
        count += it->second;
      } else  {
        dp(damaged, reconstructed, duplicated, count, duplicated_idx, seen);
      }    
  };
  const int idx = reconstructed.size();
  if (damaged[idx] == '?') { // '?' at idx
    // Assume the '?' == '.'
    { 
      reconstructed.push_back('.'); 
      dp_wrapper_with_cache_check(duplicated_idx);
      reconstructed.pop_back();
    }
    // Assume the '?' == '#'
    {
      if (duplicated_idx < duplicated.size()) {
        for(int j = 0; j < duplicated[duplicated_idx]; j++) {
          reconstructed.push_back('#');
        }
        reconstructed.push_back('.'); // There is always a '.' after a group of damaged springs
        dp_wrapper_with_cache_check(duplicated_idx+1);
      }
    }
  } else if (damaged[idx] == '.') { // '.' at idx
    reconstructed.push_back(damaged[idx]);
    dp_wrapper_with_cache_check(duplicated_idx);
  } else if (duplicated_idx < duplicated.size()) { // '#' at idx, bit for it to be a valid pattern the number of groups of '#' based on the duplicated record must not have all been seen
    for(int j = 0; j < duplicated[duplicated_idx]; j++) {
      reconstructed.push_back('#');
    }
    reconstructed.push_back('.'); // There is always a '.' after a group of damaged springs
    dp_wrapper_with_cache_check(duplicated_idx+1);
  }
  while(reconstructed.size() > idx) { // revert to original state
    reconstructed.pop_back();
  }
  seen[Key(reconstructed.size(), duplicated_idx)] = count - count_on_entry; // Update cache
  return;
}

Record parse(const std::string& line) {
  const auto separator = line.find(' ');
  Record record;
  record.damaged = line.substr(0, separator);
  record.duplicated = extract_numbers(line.substr(separator + 1, line.size() - separator - 1));
  return record;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Record> records;
  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    const auto folded_record = parse(line);
    Record unfolded_record;
    unfolded_record.damaged.append(folded_record.damaged);
    unfolded_record.damaged.push_back('.'); // Add a '.' to ensure that the last value in the input is a '.'
    unfolded_record.duplicated.insert(std::end(unfolded_record.duplicated), std::begin(folded_record.duplicated), std::end(folded_record.duplicated));
    records.emplace_back(unfolded_record);
  }

  std::size_t total = 0;
  for (const auto& record : records) {
    std::size_t count = 0;
    std::vector<char> reconstructed;
    std::unordered_map<Key, std::size_t, hasher> seen;
    dp(record.damaged, reconstructed, record.duplicated, count, 0, seen);
    total += count;
  }
  std::cout << total << '\n';

  return 0;
}
