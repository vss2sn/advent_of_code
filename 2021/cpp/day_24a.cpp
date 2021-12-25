#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>
#include <vector>

// Can be refactored to not use instruction class as
// all the relevant instructions only have a number as the second value
// So no need to store instructions etc
// But leaving as is for simplicity/posterity

bool is_digit(const std::string& s) {
  for (const char &ch : s) {
    if (!std::isdigit(ch) && ch != '-') {
      return false;
    }
  }
  return true;
}

struct Instr {
  std::string instr = "";
  std::string s1 = "";
  std::string s2 = "";
  int n2 = 0;

  Instr(const std::string& instr, const std::string& s1, const std::string& s2, const int n2) :
   instr(instr), s1(s1), s2(s2), n2(n2) {}

  friend std::ostream& operator << (std::ostream& os, const Instr& instr);

  bool operator != (const Instr& i) const {
    return instr != i.instr || s1 != i.s1 || s2 != i.s2 || n2 != i.n2;
  }
};

std::ostream& operator << (std::ostream& os, const Instr& instr) {
  os << instr.instr << ' ' << instr.s1 <<  ' ';
  if (instr.  s2 != "") os << instr.s2 << ' ';
  else os << instr.n2 << ' ';
  return os;
}

template<typename T>
void print(const std::vector<T>& v) {
  for (const auto& ele : v) {
    std::cout << ele;
  }
  std::cout << '\n';
}

void display_differences(const std::vector<Instr>& instructions) {
  for (int i = 0; i < instructions.size()/14; i++) {
    std::cout << instructions[i] << '\n';
    for (int j = 0; j < 14; j++) {
      if (instructions[j * instructions.size()/14 + i] != instructions[i]) {
        std::cout << "    For " << j << "th significant digit ----> " << instructions[j*instructions.size()/14+i] << '\n';
      }
    }
  }
}

std::vector<int> find_largest_number(const std::vector<Instr>& instructions) {
  const auto n = instructions.size() / 14;
  auto digits = std::vector<int>(14, 9);
  std::stack<std::tuple<int, int>> s;
  for (int i = 0; i < 14; i++) {
    const auto divisor = instructions[i * n + 4].n2;
    if (divisor == 1) {
      s.emplace(i, instructions[i * n + 15].n2);
    } else if (divisor == 26) {
      const auto [popped_i, to_add] = s.top();
      s.pop();
      const auto to_add_2 = instructions[i * n + 5].n2;
      digits[i] = digits[popped_i] + to_add + to_add_2;
      if (digits[i] > 9) {
        digits[popped_i] -= (digits[i] - 9);
        digits[i] = 9;
      }
      if (digits[i] < 1) {
        digits[popped_i] += (1 - digits[i]);
        digits[i] = 1;
      }
    }
  }
  return digits;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Instr> instructions;
  while(std::getline(file, line)) {
    const auto p1 = line.find(' ');
    const auto p2 = line.find(' ', p1 + 1);
    std::string s1 = "";
    std::string s2 = "";
    long long n2 = 0;
    if (p2 != std::string::npos) {
      s1 = line.substr(p1 + 1, p2 - p1 - 1);
      s2 = line.substr(p2 + 1, line.size() - p2 - 1);
      if (is_digit(s2)) {
        n2 = std::stoll(s2);
        s2 = "";
      }
    } else {
      s1 = line.substr(p1 + 1, line.size() - p1 - 1);
    }
    instructions.emplace_back(line.substr(0, p1), s1, s2, n2);
  }

  // Display variation in instructions
  // display_differences(instructions);
  const auto ans = find_largest_number(instructions);
  print(ans);
  return 0;
}
