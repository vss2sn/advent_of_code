#include <array>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>  
#include <algorithm>

const auto extract_sector_id_and_check_sum = [](std::string_view rng) {
  const auto it_begin = std::ranges::find(rng, '[');
  const auto it_end = std::ranges::find(rng, ']');
  int sector_id = 0;
  for (auto it = rng.begin(); it < it_begin; it = std::next(it)) {
    sector_id = sector_id * 10 + (*it - '0');
  }
  return std::make_pair<int, std::string_view>(
    std::move(sector_id),
    std::string_view(std::next(it_begin), std::distance(it_begin, it_end)-1)
  );
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_04_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string encrypted_name;
  int total = 0;
  while(std::getline(file, encrypted_name)) {
    std::vector<std::string_view> sub_encrypted_names;
    for (auto sub_encrypted_name : encrypted_name 
        | std::ranges::views::split('-') 
        | std::ranges::views::transform([](auto rng) {return std::string_view(rng);})) {
      sub_encrypted_names.push_back(sub_encrypted_name);
    }
    const auto [sector_id, check_sum] = extract_sector_id_and_check_sum(sub_encrypted_names.back());
    
    std::vector<std::pair<char, int>> frequency;
    for (std::size_t idx = 0; idx < sub_encrypted_names.size() - 1; idx++) {
    //   std::cout << sub_encrypted_names[idx] << '\n';
      for (const auto c : sub_encrypted_names[idx]) {
        if(auto it = std::ranges::find_if(frequency, [c](const auto& ele) { return ele.first == c; }); it != std::end(frequency)) {
          it->second++;
        } else {
          frequency.emplace_back(c, 1);
        }
      }
    }
    std::ranges::partial_sort(frequency, 
                      std::begin(frequency) + 5, 
                      [](const auto& ele_1, const auto& ele_2) {
                        return ele_1.second > ele_2.second || 
                               (ele_1.second == ele_2.second &&  
                                  ele_1.first < ele_2.first); 
                      }
                     );
    bool real_room = true;
    for (std::size_t idx = 0; idx < 5; idx++) {
      real_room = real_room && (frequency[idx].first == check_sum[idx]);
    }
    if (!real_room) continue;
    const int moves = sector_id % 26;
    // std::cout << "Moves: " << moves << '\n';
    std::string new_name = "";    
    for (std::size_t idx = 0; idx < sub_encrypted_names.size() - 1; idx++) {
    //   std::cout << sub_encrypted_names[idx] << '-';
      for (auto& c : sub_encrypted_names[idx]) {
        new_name += ('z' - c < moves) ? c + moves - 26 : c + moves;
      }
      if (idx != sub_encrypted_names.size() - 2) new_name += ' ';
    }
    // Had to scan throught the output to find the line containing north pole
    if (new_name.find("northpole") != std::string::npos) {
        // std::cout << new_name << ": " << sector_id << '\n';
        std::cout << sector_id << '\n';
        return 0;
    }
    // std::cout << " --> " << new_name << ": " << sector_id << '\n';
  }
  
  return 0;
}