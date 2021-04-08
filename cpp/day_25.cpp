#include <algorithm>
#include <cassert>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

long long FindLoopSize(const long long public_key,
                       const long long subject_number) {
  long long count = 0;
  long long value = 1;
  while (true) {
    count += 1;
    value *= subject_number;
    value %= 20201227;
    if (value == public_key) {
      break;
    }
  }
  return count;
}

long long GenerateEncrytionKey(const long long subject_number,
                               const long long loop_size) {
  long long value = 1;
  for (int i = 0; i < loop_size; i++) {
    value *= subject_number;
    value %= 20201227;
  }
  return value;
}

int main() {
  std::fstream file{"../input/day_25_input"};
  std::string line;

  std::getline(file, line);
  line.erase(std::remove_if(std::begin(line), std::end(line),
                            [](const char c) { return !isprint(c); }),
             std::end(line));
  const long long card_public_key = std::stoi(line);
  const long long card_subject_number = 7;
  const long long card_loop_size =
      FindLoopSize(card_public_key, card_subject_number);

  std::getline(file, line);
  line.erase(std::remove_if(std::begin(line), std::end(line),
                            [](const char c) { return !isprint(c); }),
             std::end(line));
  const long long door_public_key = std::stoi(line);
  const long long door_subject_number = 7;
  const long long door_loop_size =
      FindLoopSize(door_public_key, door_subject_number);

  long long encryption_key =
      GenerateEncrytionKey(card_public_key, door_loop_size);
  assert(encryption_key ==
         GenerateEncrytionKey(door_public_key, card_loop_size));
  std::cout << encryption_key << '\n';
  return encryption_key;
}
