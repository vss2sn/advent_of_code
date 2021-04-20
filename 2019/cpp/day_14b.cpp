#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

long long OreRequired(
    const long long fuel,
    std::unordered_map<
        std::string,
        std::pair<long long, std::unordered_map<std::string, long long>>>&
        reactions) {
  std::unordered_map<std::string, long long> to_create{{"FUEL", fuel}};
  std::unordered_map<std::string, long long> spares;
  long long ore_units = 0;

  while (!to_create.empty()) {
    const auto product = std::begin(to_create)->first;
    if (to_create[product] < spares[product]) {
      spares[product] -= to_create[product];
      to_create.erase(product);
      continue;
    }
    const auto units_needed = to_create[product] - spares[product];
    spares[product] = 0;
    to_create.erase(product);
    const auto units_produced_per_reaction = reactions[product].first;

    long long num_reactions = 0;
    if (units_needed % units_produced_per_reaction == 0) {
      num_reactions = units_needed / units_produced_per_reaction;
    } else {
      num_reactions = (units_needed / units_produced_per_reaction) + 1;
    }

    spares[product] +=
        (num_reactions * units_produced_per_reaction) - units_needed;
    for (const auto& [reactant, reactant_units] : reactions[product].second) {
      if (reactant == "ORE") {
        ore_units += reactant_units * num_reactions;
      } else {
        to_create[reactant] += reactant_units * num_reactions;
      }
    }
  }
  return ore_units;
}

std::tuple<std::string, int> GetChemicalAndQuantity(const std::string& input) {
  const auto space_ind = input.find(' ');
  const auto quantity = std::stoi(input.substr(0, space_ind));
  const auto ingredient = input.substr(space_ind + 1, input.size() - space_ind);
  return {ingredient, quantity};
}

std::unordered_map<
    std::string,
    std::pair<long long, std::unordered_map<std::string, long long>>>
ParseInput(const std::vector<std::string>& input_file) {
  std::unordered_map<
      std::string,
      std::pair<long long, std::unordered_map<std::string, long long>>>
      reactions;
  for (const auto& input_str : input_file) {
    const std::string delimiter1 = " => ";
    const std::size_t delim_ind = input_str.find(delimiter1);
    const auto output =
        input_str.substr(delim_ind + delimiter1.size(),
                         input_str.size() - delim_ind - delimiter1.size());
    const auto out_chem_q = GetChemicalAndQuantity(output);
    reactions[std::get<0>(out_chem_q)] = {};
    reactions[std::get<0>(out_chem_q)].first = std::get<1>(out_chem_q);

    const auto inputs = input_str.substr(0, delim_ind);
    const std::string delimiter2 = ", ";
    size_t start = 0;
    size_t end = inputs.find(delimiter2);
    while (end != std::string::npos) {
      const auto in_chem_q =
          GetChemicalAndQuantity(inputs.substr(start, end - start));
      reactions[std::get<0>(out_chem_q)].second.insert(
          {std::get<0>(in_chem_q), std::get<1>(in_chem_q)});
      start = end + delimiter2.size();
      end = inputs.find(delimiter2, start);
    }
    const auto in_chem_q =
        GetChemicalAndQuantity(inputs.substr(start, end - start));
    reactions[std::get<0>(out_chem_q)].second.insert(
        {std::get<0>(in_chem_q), std::get<1>(in_chem_q)});
  }
  return reactions;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;
  std::vector<std::string> input_file;
  while (std::getline(file, input_line)) {
    input_file.push_back(input_line);
  }

  // Solve
  auto reactions = ParseInput(input_file);

  constexpr long long ore_available = 1000000000000;
  const auto ore_for_1_fuel = OreRequired(1, reactions);
  auto low = ore_available / ore_for_1_fuel;
  auto high = low * 10;
  while (OreRequired(high, reactions) < ore_available) {
    low = high;
    high = 10 * low;
  }

  long long mid = 0;
  while (low + 1 < high) {
    mid = (low + high) / 2;
    long long cost = OreRequired(mid, reactions);
    if (cost > ore_available) {
      high = mid;
    } else if (cost < ore_available) {
      low = mid;
    } else {
      std::cout << mid << '\n';
      return mid;
    }
  }
  long long final = high;
  if (OreRequired(final, reactions) > ore_available) {
    final--;
  }
  std::cout << final << '\n';
  return final;
}
