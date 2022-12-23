#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

enum class YellType {
  NUMBER,
  RESULT
};

struct Monkey {
  long long id;
  std::string name;
  YellType type;
  long long val = 0;
  std::string dep1;
  std::string dep2;
  char op;
  bool calced = false;
};

long long calc_result(long long num1, long long num2, char op) {
  if (op == '+') {
    return num1 + num2;
  }
  if (op == '-') {
    return num1 - num2;
  }
  if (op == '*') {
    return num1 * num2;
  }
  if (op == '/') {
    // assert (num1 / num2 == float(num1) / float(num2));
    return num1 / num2;
  }
  else {
    std::cout << "This should not happen" << '\n';
    exit(0);
  }
}


int main(int argc, char * argv[]) {
  std::string input = "../input/day_21_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);

  std::vector<Monkey> monkeys;
  std::unordered_map<std::string, long long> monkey_mapping;

  long long id = 0;
  while(std::getline(file, line)) {
    auto m = Monkey();
    m.id = id;
    m.name = line.substr(0, 4);
    // std::cout << m.name << ": " ;
    if (line[6] >= '0' && line[6] <= '9') {
      m.type = YellType::NUMBER;
      m.val = std::stoll(line.substr(6, line.size() - 6));
      // std::cout << m.val << '\n';
    } else {
      m.type = YellType::RESULT;
      auto start = 6;
      auto end = line.find(' ' , start);
      m.dep1 = line.substr(start, end - start);
      start = end+1;
      m.op = line[start];
      start+=2;
      m.dep2 = line.substr(start, line.size() - start);
      // std::cout << m.dep1 << ' '  << m.op << ' ' << m.dep2 << '\n';
    }
    m.calced = false;
    monkeys.push_back(m);
    monkey_mapping[m.name] = id;
    id++;
  }

  long long count = 0;
  while (true) {
    for (auto& monkey : monkeys) {
      // std::cout << "Monkey " << monkey.name << '\n';
      if (monkey.calced) continue;
      if (monkey.type == YellType::RESULT) {
        if (!monkeys[monkey_mapping[monkey.dep1]].calced) {
          // std::cout << monkey.dep1 <<  " has not run its calc yet " << '\n';
          continue;
        }
        if (!monkeys[monkey_mapping[monkey.dep2]].calced) {
          // std::cout << monkey.dep2 <<  " has not run its calc yet " << '\n';
          continue;
        }

        monkey.val = calc_result(
          monkeys[monkey_mapping[monkey.dep1]].val,
          monkeys[monkey_mapping[monkey.dep2]].val,
          monkey.op
        );
        monkey.calced = true;
        // std::cout << monkey.name <<  " ran its calc " << '\n';
        if (monkey.name == "root") {
          // std::cout << monkeys[monkey_mapping[monkey.dep1]].val << '\n';
          // std::cout << monkeys[monkey_mapping[monkey.dep2]].val << '\n';
          std::cout << monkey.val << '\n';
          return 0;
        }
      } else {
        // std::cout << monkey.name <<  " ran its calc " << '\n';
        monkey.calced = true;
      }
    }
    count++;
  }
  return 0;
}
