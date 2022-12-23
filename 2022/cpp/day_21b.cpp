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
    return num1 / num2;
  }
  if (op == '=') {
    return num1 == num2;
  }
  else {
    std::cout << "This should not happen" << '\n';
    exit(0);
  }
}

long long reverse_calc_result(long long num1, long long num2, char op) {
  if (op == '+') {
    return num1 - num2;
  }
  if (op == '-') {
    return num1 + num2;
  }
  if (op == '*') {
    return num1 / num2;
  }
  if (op == '/') {
    return num1 * num2;
  }
  if (op == '=') {
    return num2;
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
      m.val = std::stoi(line.substr(6, line.size() - 6));
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
    if (m.name == "root") {
      m.op = '=';
      m.calced = true;
      m.type = YellType::RESULT;
    }
    if (m.name == "humn") {
      m.op = '?';
      m.type = YellType::NUMBER;
    }
    monkeys.push_back(m);
    monkey_mapping[m.name] = id;
    id++;
  }

  long long count = 0;
  while (true) {
    for (auto& monkey : monkeys) {
      // std::cout << "Monkey " << monkey.name << '\n';
      // std::cout << monkey.calced << '\n';
      if (!monkey.calced && monkey.name != "humn") {
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
        } else {
          // std::cout << monkey.name <<  " ran its calc " << '\n';
          monkey.calced = true;
        }
      }
      if (monkey.calced && monkey.type == YellType::RESULT) {
        int mi1 = monkey_mapping[monkey.dep1];
        int mi2 = monkey_mapping[monkey.dep2];
        // std::cout << mi1 << ' ' << mi2 << '\n';
        if ((monkeys[mi1].calced) && (monkeys[mi2].calced)) {
          // std::cout << "Both " << monkeys[mi1].name << " and " << monkeys[mi2].name << " are known" << '\n';
          continue;
        } else if ((monkeys[mi1].calced) && (!monkeys[mi2].calced)) {
          monkeys[mi2].val = reverse_calc_result(monkey.val, monkeys[mi1].val, monkey.op);
          if (monkey.op == '/') {
            monkeys[mi2].val = monkeys[mi1].val / monkey.val;
          }
          if (monkey.op == '-') {
            monkeys[mi2].val = monkeys[mi1].val - monkey.val;
          }
          monkeys[mi2].calced = true;
        } else if ((!monkeys[mi1].calced) && (monkeys[mi2].calced)) {
          monkeys[mi1].val = reverse_calc_result(monkey.val, monkeys[mi2].val, monkey.op);

          monkeys[mi1].calced = true;
        } else if (!(monkeys[mi1].calced) && !(monkeys[mi2].calced)) {
          // std::cout << "Both " << monkeys[mi1].name << " and " << monkeys[mi2].name << " are unknown" << '\n';
          continue;
        }
        // assert(monkeys[mi1].type == YellType::RESULT || monkeys[mi2].type == YellType::RESULT);
        // if (monkey.name == "root") {
        //   std::cout << monkey.name << '\n';
        //   std::cout << "Root used " << '\n';
        //   std::cout << monkeys[mi1].calced << ' ' << monkeys[mi2].calced << '\n';
        //   std::cout << monkeys[mi1].val << ' ' << monkeys[mi2].val << '\n';
        //   // exit(0);
        // }
        // std::cout << monkey.val << '\n';
        // std::cout << monkeys[mi1].val << '\n';
        // std::cout << monkeys[mi2].val << '\n';
      }
      if (monkeys[monkey_mapping["humn"]].calced) {
        // std::cout << "Human should shout ";
        std::cout << monkeys[monkey_mapping["humn"]].val << '\n';
        return 0;
      }
    }

    count++;
  }
  return 0;
}
