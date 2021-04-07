#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

std::vector<std::string> parse(const std::string& data, const std::string& delimiters = " ") {
  size_t start = 0;
  size_t end = data.find_first_of(delimiters);
  std::vector<std::string> parsed;

  while(end != std::string::npos) {
    std::string data_substr = data.substr(start, end - start);
    data_substr.erase(std::remove_if(std::begin(data_substr), std::end(data_substr), [](const char c) {return !isprint(c);}), std::end(data_substr));
    if(!data_substr.empty() && data.substr(start, 1) != " ") {
      parsed.push_back(data.substr(start, end - start));
    }
    start = end + 1;
    end = data.find_first_of(delimiters, start);
  }
  if(start < data.size()) {
    parsed.push_back(data.substr(start, end - start));
  }
  return parsed;
}

int main() {
  std::fstream file{"../input/day_21_input"};
  std::string line;
  const std::string delimiter = "(contains ";
  std::unordered_map<std::string, std::unordered_set<std::string>> allergen_map;
  std::unordered_map<std::string, int> ingredients_count;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](const char c) {return !isprint(c);}), std::end(line));
    if(line == "") continue;
    const auto con_ind = line.find(delimiter);
    std::vector<std::string> ingredients = parse(line.substr(0, con_ind));
    std::vector<std::string> allergens = parse(line.substr(con_ind + delimiter.size(), line.size() - con_ind - delimiter.size()), ", )");
    // Count the number of times an ingredient has been used
    for(const auto& ingredient : ingredients) {
      if(ingredients_count.find(ingredient) == ingredients_count.end()) {
        ingredients_count[ingredient] = 1;
      } else {
        ingredients_count[ingredient]++;
      }
    }
    for (const auto& allergen : allergens) {
      // For every allergen that has not been seen before add the ingredients that could potentially contain it
      if(allergen_map.find(allergen) == allergen_map.end()) {
        allergen_map[allergen] = {};
        for(const auto& ingredient : ingredients) {
          allergen_map[allergen].insert(ingredient);
        }
      } else {
        // For every allergen that has been seen before remove the ingredients that could potentially contain it
        std::unordered_set<std::string> subset;
        for(const auto& potential_ingredient : allergen_map[allergen]) {
          bool found = false;
          for(const auto& ingredient : ingredients) {
            if(ingredient == potential_ingredient) {
              found = true;
              break;
            }
          }
          if(found) {
            subset.insert(potential_ingredient);
          }
        }
        allergen_map[allergen] = subset;
      }
    }
  }

  std::unordered_set<std::string> allergen_free_ingredients;
  std::unordered_map<std::string, std::string> known_allergens;
  std::vector<std::pair<std::string, std::string>> known_allergic_ingredients;
  bool changed = true;
  while(changed) {
    changed = false;
    std::string allergen_to_remove = "", ingredient_to_remove = "";
    for (const auto& [allergen, potential_ingredients] : allergen_map) {
      if(potential_ingredients.size() == 1) {
        ingredient_to_remove = *std::begin(potential_ingredients);
        allergen_to_remove = allergen;
        known_allergens.insert({allergen_to_remove, ingredient_to_remove});
        known_allergic_ingredients.emplace_back(ingredient_to_remove, allergen_to_remove);
        changed = true;
        break;
      }
    }
    if(allergen_to_remove == "") {
      break;
    }
    allergen_map.erase(allergen_to_remove);
    for(auto& [allergen, potential_ingredients] : allergen_map) {
      if (potential_ingredients.find(ingredient_to_remove) != potential_ingredients.end()) {
        potential_ingredients.erase(ingredient_to_remove);
      }
    }
  }
  std::sort(std::begin(known_allergic_ingredients), std::end(known_allergic_ingredients), [](const auto& ele1, const auto& ele2) {return ele1.second < ele2.second; } );
  std::string ans_str = "";
  for(const auto& [ingredient, allergen] : known_allergic_ingredients) {
    ans_str += ingredient + ',';
  }
  ans_str.erase(std::end(ans_str)-1);
  std::cout << ans_str << '\n';
  return 0;
}
