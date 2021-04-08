#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

int main() {
  std::ifstream file{"../input/day_13_input"};

  int arrive_abs;  // attive at bus stop
  file >> arrive_abs;

  std::string all_bus_f;  // all buses frequencies
  file >> all_bus_f;

  int earliest_bus;             // To be found
  int extra_time = arrive_abs;  // Ensures > arrive_abs % all_bus_f
  int begin = 0;
  int end = all_bus_f.find(",", begin);
  while (end != std::string::npos) {
    const std::string bus_f = all_bus_f.substr(begin, end - begin);
    if (bus_f.find_first_not_of("1234567890") == std::string::npos) {
      std::cout << bus_f << '\n';
      const int temp = std::abs((arrive_abs % stoi(bus_f)) - stoi(bus_f));
      std::cout << "Pt extra time: " << temp << '\n';

      if (temp < extra_time) {
        extra_time = temp;
        earliest_bus = stoi(bus_f);
      }
    }
    begin = end + 1;
    end = all_bus_f.find(",", begin);
  }
  const std::string bus_f = all_bus_f.substr(begin, end - begin);
  if (bus_f.find_first_not_of("1234567890") == std::string::npos) {
    std::cout << "Bus: " << bus_f << '\n';
    const int temp = std::abs((arrive_abs % stoi(bus_f)) - stoi(bus_f));
    std::cout << "Pt extra time: " << temp << '\n';
    if (temp < extra_time) {
      extra_time = temp;
      earliest_bus = stoi(bus_f);
    }
  }
  std::cout << earliest_bus << '\n';
  std::cout << extra_time << '\n';
  std::cout << earliest_bus * extra_time << '\n';
  return earliest_bus * extra_time;
}
