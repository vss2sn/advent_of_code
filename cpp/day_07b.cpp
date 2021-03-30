#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int findTotalBags(const std::string& colour,
  const std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>& contains,
  int number_of_current_bag) {
  int count = 0;
  for(const auto& [number, colour] : contains.at(colour)) {
    if(contains.find(colour) != contains.end()) {
      count += findTotalBags(colour, contains, number);
    }
  }
  return (count + 1) * number_of_current_bag;
}

int main() {
  std::ifstream file;
  file.open("day_7_data.txt");
  std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> contains;
  std::unordered_map<std::string, std::vector<std::string>> can_be_contained_by;
  std::string rule;
  while(std::getline(file, rule)) {
    rule.erase(std::remove_if(std::begin(rule), std::end(rule), [](auto c) { return !isprint(c); }), std::end(rule) );

    size_t start = 0;
    size_t end = rule.find(" contain ");
    size_t delta =  9;

    // Extract large bag
    std::string large_bag = rule.substr(0, end);

    // Extract large bag's colour
    const std::string lb_colour = large_bag.substr(0, large_bag.find_last_of(' '));
    contains.insert({lb_colour, {}});

    // Extract all small bags
    std::string small_bags = rule.substr(end + delta, rule.size() - end - delta);

    start = 0;
    end = small_bags.find_first_of(",.");
    while(end != std::string::npos) {

      // Get small bag details
      std::string small_bag = small_bags.substr(start, end - start);
      small_bag = small_bag.substr(small_bag.find_first_not_of(' '), small_bag.size() - small_bag.find_first_not_of(' '));
      if(small_bag == "no other bags") {
        break;
      }

      // Get colour of small bag
      size_t colour_begin = small_bag.find(' ');
      const int number_of_small_bag = std::stoi(small_bag.substr(0, colour_begin));
      const std::string colour = small_bag.substr(colour_begin + 1, small_bag.find_last_of(' ') - colour_begin - 1);

      // Insert into contain map
      contains[lb_colour].push_back({number_of_small_bag, colour});

      // Insert into can be contained by map
      can_be_contained_by.insert({colour, {}});
      can_be_contained_by[colour].push_back(lb_colour);

      start = end + 1;
      end = small_bags.find_first_of(",.", start);
    }
  }

  int number_of_current_bag = 1;
  std::string colour = "shiny gold";
  int count = findTotalBags(colour, contains, number_of_current_bag) - 1; // -1 for the bag being searched for
  std::cout << count << '\n';
  return count;
}
