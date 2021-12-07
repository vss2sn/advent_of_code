#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

std::pair<std::string, std::array<unsigned long long, 3>> parse_input(const std::string& s_main) {
  std::string s = s_main;
  std::array<unsigned long long, 3> nums;
  std::string delimiter = " ";
  unsigned long long start = 0;
  auto end = s.find(delimiter);
  const auto instr = s.substr(start, end - start);
  start = end + 1;
  end = s.find(delimiter, start);
  unsigned long long count = 0;
  while (end != std::string::npos) {
    nums[count] = std::stoi(s.substr(start, end - start));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
    count++;
  }
  nums[count] = std::stoi(s.substr(start, s.size() - start));
  return {instr, nums};
}

int main(int argc, char * argv []) {

  // Helper code to transtlate instructions into human readable ones
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  const std::unordered_map<std::string, std::function<void(const unsigned long long, const unsigned long long, const unsigned long long)>> translator
   {
    {"addr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " + R" << b << '\n'; } } },
    {"addi", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " + " << b << '\n'; } } },
    {"mulr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " *  R" << b << '\n'; } } },
    {"muli", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " * " << b << '\n'; } } },
    {"banr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " &  R" << b << '\n'; } } },
    {"bani", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " & " << b << '\n'; } } },
    {"borr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " |  R" << b << '\n'; } } },
    {"bori", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " | " << b << '\n'; } } },
    {"setr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << '\n'; } } },
    {"seti", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = " << a << '\n'; } } },
    {"gtir", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = " << a << " > R" << b << '\n';} } },
    {"gtri", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " > " << b << '\n'; } } },
    {"gtrr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " > R" << b << '\n'; } } },
    {"eqir", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = " << a << " == R" << b<< '\n'; } } },
    {"eqri", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " == " << b << '\n';} } },
    {"eqrr", { [](const unsigned long long a, const unsigned long long b, const unsigned long long c) { std::cout << "R" << c << " = R" << a << " == R" << b << '\n'; } } }
  };

  std::getline(file, line);
  std::cout << "--- Translated instructions ---" << '\n';
  while(std::getline(file, line)) {
    const auto [instr, args] = parse_input(line);
    const auto it = translator.find(instr);
    it->second(args[0], args[1], args[2]);
  }
  std::cout << "--- End translated instructions ---" << '\n';

  unsigned long long sum = 0;
  constexpr unsigned long long magic_number = 10551306;
  constexpr unsigned long long sqrt_magic_number = std::sqrt(magic_number);
  for (unsigned long long i = 1; i < sqrt_magic_number; i++) {
    if (magic_number % i == 0) {
      if (i == sqrt_magic_number) {
        sum += i;
      } else {
        sum += i + magic_number / i;
      }
    }
  }
  std::cout << sum << '\n';
  return 0;
}

// Translated instructions with comments
// R1 = R1 + 16
// R5 = 1 // Start loop 1
// R2 = 1 // Start loop 2
// R3 = R5 * R2
// R3 = R3 == R4
// R1 = R3 + R1
// R1 = R1 + 1
// R0 = R5 + R0 // if R5 * R2 == R3, add R3 to R0. Ie if R5 is a factor of R3, add to R0
// R2 = R2 + 1
// R3 = R2 > R4
// R1 = R1 + R3
// R1 = 2 // continue loop if R4 <= R2
// R5 = R5 + 1
// R3 = R5 > R4
// R1 = R3 + R1
// R1 = 1 // continue loop if R5 <= R4
// R1 = R1 * R1 // Go out of bounds once above loops are complere
// R4 = R4 + 2 // From here including this line
// R4 = R4 *  R4
// R4 = R1 *  R4
// R4 = R4 * 11
// R3 = R3 + 3
// R3 = R3 *  R1
// R3 = R3 + 4
// R4 = R4 + R3
// R1 = R1 + R0
// R1 = 0 // Skipped
// R3 = R1
// R3 = R3 *  R1
// R3 = R1 + R3
// R3 = R1 *  R3
// R3 = R3 * 14
// R3 = R3 *  R1
// R4 = R4 + R3 // Till here including this line, set R4 = 10551306
// R0 = 0
// R1 = 0
