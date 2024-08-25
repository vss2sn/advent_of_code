#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

constexpr int n_properties = 5;
constexpr int max_teaspoons = 100;
constexpr int n_calories = 500;
struct Ingredient {
    std::string name;
    std::array<int, n_properties> stats;
};

int sum(const std::vector<int>& teaspoons) {
    int total = 0;
    for (const auto ele : teaspoons) {
        total += ele;
    }
    return total;
}

// Room for optimization here
bool next_option (std::vector<int>& teaspoons, int& index, int& max_index, const int n_ingredients) {
    if (max_index >= n_ingredients) return false;
    if (index >= n_ingredients) return false;
    if (sum(teaspoons) < max_teaspoons) {
        teaspoons[index]++;
        return true;
    }
    if(index == max_index && teaspoons[max_index] == max_teaspoons) {
        max_index++;
        if (max_index == n_ingredients) return false;
        index = 0;
        for (auto& ele : teaspoons) {
            ele = 0;
        }
        teaspoons[max_index] = 1;
        return true;
    } 
    index++;
    for (int i = 0; i < index; i++) {
        teaspoons[i] = 0;
    }
    teaspoons[index] += 1;
    if (sum(teaspoons) == max_teaspoons) return next_option(teaspoons, index, max_index,n_ingredients);
    index = 0;
    return true;
}

int calc_score(std::vector<int>& teaspoons, std::vector<Ingredient>& ingredients, const int n_ingredients) {
    std::array<int, n_properties> score = {0,0,0,0,0};
    for (int i = 0; i < n_ingredients; i++) {
        for (int j = 0; j < n_properties; j++) {
            score[j] += teaspoons[i] * ingredients[i].stats[j];
        }
    }
    int total_score = 1
    // Assumes calories is the last property;
    for (int j = 0; j < n_properties - 1; j++) {
        if (score[j] < 0) return 0;
        total_score *= score[j];
    }
    if (score[n_properties-1] != n_calories) total_score = 0;
    return total_score;
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_15_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    const std::regex pattern(R"(([a-zA-Z]+): capacity (-?[0-9]+), durability (-?[0-9]+), flavor (-?[0-9]+), texture (-?[0-9]+), calories (-?[0-9]+))");
    std::vector<Ingredient> ingredients;
    while(std::getline(file, line)) {
        std::smatch match;
        std::regex_search(line, match, pattern);
        Ingredient i;
        // std::cout << match.size() << '\n'; 
        // for (const auto ele : match) {
        //     std::cout << '|' << ele << '|'<<'\n';
        // }
        i.name = match[1];
        i.stats[0] = std::stoi(match[2]);
        i.stats[1] = std::stoi(match[3]);
        i.stats[2] = std::stoi(match[4]);
        i.stats[3] = std::stoi(match[5]);
        i.stats[4] = std::stoi(match[6]);
        ingredients.push_back(i);
    }
    int index = 0;
    int max_index = 0;
    int max_score = 0;
    const int n_ingredients = ingredients.size();
    std::vector<int> teaspoons(n_ingredients, 0);
    while(next_option(teaspoons, index, max_index, n_ingredients)) {
        max_score = std::max(calc_score(teaspoons, ingredients, n_ingredients), max_score);
        // for (const auto ele : teaspoons) {
        //     std::cout << ele << ' ';
        // }
        // std::cout << '\n';
    }
    std::cout << max_score << '\n';
}   
