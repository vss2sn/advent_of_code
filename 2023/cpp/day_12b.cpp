#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Key {
  int reconstructed_size;
  int duplicated_idx;

  Key(const int reconstructed_size,
      const int duplicated_idx) 
      : reconstructed_size(reconstructed_size), duplicated_idx(duplicated_idx) {}

  bool operator == (const Key& k) const {
    return reconstructed_size == k.reconstructed_size && duplicated_idx == k.duplicated_idx;
  }
};

struct hasher {
  std::size_t operator () (const Key& p) const {
    return p.reconstructed_size * p.duplicated_idx;
  }
};

struct Record {
  std::string damaged;
  std::vector<int> duplicated;
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

void print(const std::vector<char>& v) {
  for (const auto ele : v) {
    std::cout << ele;
  }
}

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

void dp(const std::string& damaged, std::vector<char>& reconstructed, const std::vector<int>& duplicated, std::size_t& count, const int duplicated_i, std::unordered_map<Key, std::size_t, hasher>& seen) {
  if ( const auto it = seen.find(Key(reconstructed.size(), duplicated_i)); it != seen.end()) {
    count += it->second;
    return;
  }

  const std::size_t count_on_entry_main = count;
  // std::cout << "reconstructed: ";
  // print(reconstructed);
  // std::cout << '\n';
  
  if (duplicated_i > duplicated.size() + 1) {
    // std::cout << duplicated_i << ' '<<  duplicated.size() + 1 << '\n';
    // std::cout << "Number of damaged sets exceeds number in records" << '\n';
    return;
  }
  
  if (!compare(damaged, reconstructed)) {
    // std::cout << "Comparison with damaged failed" << '\n';
    // std::cout << damaged << '\n';
    // print(reconstructed);
    // std::cout << '\n';
    return;
  }

    if (!compare_with_duplicated(reconstructed, duplicated)) {
      // std::cout << "Comparison with duplicated failed" << '\n';
      return ;
    }

  if (damaged.size() == reconstructed.size() && compare(damaged, reconstructed)) {
    // std::cout << "Size match" << '\n';
    if (duplicated_i == duplicated.size()) {
      // std::cout << damaged << " | ";
      // print(reconstructed);
      // std::cout << '\n';
      count++;
    }
    // std::cout << "Size of reconstructed == size of damaged record" << '\n';
    return;
  }

  if (std::begin(duplicated) + duplicated_i <= std::end(duplicated) && ((std::accumulate(std::begin(duplicated) + duplicated_i, std::end(duplicated), 0) + duplicated.size() - duplicated_i) > (damaged.size() - reconstructed.size())) ) {
    // std::cout << "Not possible as number of damaged + 1 interval between each damaged set > number remaining" << '\n';
    return;
  }

  int idx = reconstructed.size();
  const int idx_init = reconstructed.size();
  if (damaged[idx] == '?') {
    {
      reconstructed.push_back('.'); 
      if (const auto it = seen.find(Key(reconstructed.size(), duplicated_i)); it != seen.end()) {
        // print(reconstructed);
        // std::cout << '\n';
        // std::cout << __LINE__ << " Seen before" << reconstructed.size() << ' ' << duplicated_i << '\n';
        count += it->second;
      } else  {
        dp(damaged, reconstructed, duplicated, count, duplicated_i, seen);
        // print(reconstructed);
        // std::cout << '\n';
      }
      reconstructed.pop_back();
    }
    
    {
      if (duplicated_i < duplicated.size()) {
        for(int j = 0; j < duplicated[duplicated_i]; j++) {
          reconstructed.push_back('#');
        }
        reconstructed.push_back('.');
        if (const auto it = seen.find(Key(reconstructed.size(), duplicated_i + 1)); it != seen.end()) {
          // print(reconstructed);
          // std::cout << '\n';
          // std::cout << __LINE__ << " Seen before" << reconstructed.size() << ' ' << duplicated_i + 1 << '\n';
          count += it->second;
        } else {
          dp(damaged, reconstructed, duplicated, count, duplicated_i+1, seen);
          // print(reconstructed);
          // std::cout << '\n';
        }
      }
    }
  } else {
    if (damaged[idx] == '.') {
      const int count_on_entry = count;      
      reconstructed.push_back(damaged[idx]);
      if (const auto it = seen.find(Key(reconstructed.size(), duplicated_i)); it != seen.end()) {
        // print(reconstructed);
        // std::cout << '\n';
        // std::cout << __LINE__ << " Seen before" << reconstructed.size() << ' ' << duplicated_i << '\n';
        count += it->second;
      } else {
        dp(damaged, reconstructed, duplicated, count, duplicated_i, seen);
        // print(reconstructed);
        // std::cout << '\n';
      }
    } else if (duplicated_i < duplicated.size()) {
      const int count_on_entry = count;      
      for(int j = 0; j < duplicated[duplicated_i]; j++) {
        reconstructed.push_back('#');
      }
      reconstructed.push_back('.');
      if (const auto it = seen.find(Key(reconstructed.size(), duplicated_i+1)); it != seen.end()) {
        // print(reconstructed);
        // std::cout << '\n';
        // std::cout << __LINE__ << " Seen before" << reconstructed.size() << ' ' << duplicated_i+1 << '\n';
        count += it->second;
      } else {
        dp(damaged, reconstructed, duplicated, count, duplicated_i+1, seen);
        // print(reconstructed);
        // std::cout << '\n';
      }
    }
  }
  while(reconstructed.size() > idx_init) {
    reconstructed.pop_back();
  }
  // std::cout << __LINE__ << " Adding" << reconstructed.size() << ' ' << duplicated_i << ' ' << count - count_on_entry << '\n';
  seen[Key(reconstructed.size(), duplicated_i)] = count - count_on_entry_main;
  return;
}

Record parse(const std::string& line) {
  const auto separator = line.find(' ');
  Record record;
  record.damaged = line.substr(0, separator);
  // std::cout << record.damaged << " --> ";
  record.duplicated = extract_numbers(line.substr(separator + 1, line.size() - separator - 1));
  // for (const auto & ele : record.duplicated) {
  //   std::cout << ele << ' ';
  // }
  // std::cout << '\n';
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
    for (int i = 0; i < 4; i++) {
      unfolded_record.damaged.append(folded_record.damaged);
      unfolded_record.damaged.append(1, '?');
      unfolded_record.duplicated.insert(std::end(unfolded_record.duplicated), std::begin(folded_record.duplicated), std::end(folded_record.duplicated));
    }
    unfolded_record.damaged.append(folded_record.damaged);
    unfolded_record.damaged.push_back('.'); // Add a '.' to ensure that the last value in the input is a '.'
    unfolded_record.duplicated.insert(std::end(unfolded_record.duplicated), std::begin(folded_record.duplicated), std::end(folded_record.duplicated));
    // std::cout << unfolded_record.damaged << '\n';
    // for (const auto ele : unfolded_record.duplicated) {
    //   std::cout << ele << ' ' ;
    // }
    // std::cout << '\n';
    records.emplace_back(unfolded_record);
  }

  std::size_t total = 0;
  for (const auto& record : records) {
    // std::cout << record.damaged << " | ";
    // for (const auto & ele : record.duplicated) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << '\n';
    std::size_t count = 0;
    std::vector<char> reconstructed;
    std::unordered_map<Key, std::size_t, hasher> seen;
    dp(record.damaged, reconstructed, record.duplicated, count, 0, seen);
    // std::cout << "Count: " << count << '\n';
    total += count;
    // exit(0);
  }
  std::cout << total << '\n';

  return 0;
}

