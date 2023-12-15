#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Record {
  std::string damaged;
  std::vector<int> duplicated;
  int n_spaces;
};

bool compare(const std::string& damaged, const std::string& from_duplicate) {
  for (int i = 0; i < from_duplicate.size(); i++) {
    if (damaged[i] == '?') continue;
    else if (from_duplicate[i] != damaged[i]) {
      // std::cout << "false" << '\n';
      return false;
    }
  }
  // std::cout << "true" << '\n';
  return true;
}

void permutation(const int depth, const Record& record, const std::string& from_duplicate, std::size_t& count, int spaces_used) {
  // std::cout << "depth: " << depth << '\n';
  // std::cout << "from_duplicate: " << from_duplicate << '\n';
  if (!compare(record.damaged.substr(0, from_duplicate.size()), from_duplicate)) {
    return;
  }
  if (depth == record.duplicated.size()-1) {
    std::string temp = from_duplicate;
    temp.append(record.damaged.size() - from_duplicate.size(), '.');
    // std::cout << "Comparing full: " << temp << '\n';
    count += compare(record.damaged, temp);
    return;
  }
  for (int n_spaces = 1; n_spaces <= (record.n_spaces - spaces_used); n_spaces++) {
    std::string new_from_duplicate = from_duplicate;
    new_from_duplicate.append(n_spaces, '.');
    new_from_duplicate.append(record.duplicated[depth+1], '#');
    permutation(depth + 1, record, new_from_duplicate, count, spaces_used + n_spaces);
  }
}

void permutation_main(const Record& record, std::size_t& count_main) {
  int min_size_from_duplicates = record.duplicated.size() - 1; // min number of empty space
  for (const auto & n : record.duplicated) {
    min_size_from_duplicates += n; // spaces occupied by '#'
  }
  // std::cout << record.damaged.size() << ' ' << min_size_from_duplicates << '\n';
  // exit(0);
  std::size_t count = 0;
  for (int i = 0; i <= record.damaged.size() - min_size_from_duplicates; i++) {
    std::string from_duplicate = "";
    from_duplicate.append(i, '.');
    from_duplicate.append(record.duplicated[0], '#');
    // std::cout << i << ' ' << record.damaged.size() << ' ' << min_size_from_duplicates << ' ' << from_duplicate << '\n';
    permutation(0, record, from_duplicate, count, i);
  }
  // std::cout << count << '\n';
  count_main += count;
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
    records.emplace_back(parse(line));
  }
  for (auto& record : records) {
    int total = 0;
    for (const auto ele : record.duplicated) {
      total += ele;
    }
    record.n_spaces = record.damaged.size() - total;
  }

  std::size_t count = 0;
  for (const auto& record : records) {
    permutation_main(record, count);
  }
  std::cout << count << '\n';
  return 0;
}