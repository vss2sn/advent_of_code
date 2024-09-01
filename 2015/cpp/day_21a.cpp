#include <cmath>
#include <fstream>
#include <iostream>
#include <regex> 
#include <string>
#include <string_view>
#include <unordered_map> 
#include <unordered_set> 
#include <utility>
#include <vector>
#include <limits>

struct Entity {
    int hp;
    int damage;
    int armour;
};

int calculate_strikes(const Entity& striker, const Entity& struck) {
    const int damage_dealt = std::max(1, striker.damage - struck.armour);
    return std::ceil(static_cast<float>(struck.hp) / damage_dealt);
}

struct Item {
    std::string name; 
    int cost;
    int damage;
    int armour;
};

std::vector<Item> weapons {
    {"Dagger",8,4,0},
    {"Shortsword",10,5,0},
    {"Warhammer",25,6,0},
    {"Longsword",40,7,0},
    {"Greataxe",74,8,0},
};

std::vector<Item> armours {
    {"Leather",13,0,1},
    {"Chainmail",31,0,2},
    {"Splintmail",53,0,3},
    {"Bandedmail",75,0,4},
    {"Platemail",102,0,5},
    {"None",0,0,0},
};

std::vector<Item> rings {
    {"Damage+1",25,1,0},
    {"Damage+2",50,2,0},
    {"Damage+3",100,3,0},
    {"Defense+1",20,0,1},
    {"Defense+2",40,0,2},
    {"Defense+3",80,0,3},
    {"None", 0,0,0}, // Add none as possible to choose only 1 ring
    // Case for zero rings handled in main
};

int main(int argc, char * argv[]) {
    std::string input = "../input/day_21_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    Entity enemy;
    enemy.hp = std::stoi(line.substr(line.find(':') + 2, line.size()- line.find(':') - 2));
    std::getline(file, line);
    enemy.damage = std::stoi(line.substr(line.find(':') + 2, line.size()- line.find(':') - 2));
    std::getline(file, line);
    enemy.armour = std::stoi(line.substr(line.find(':') + 2, line.size()- line.find(':') - 2));
    int min_cost = std::numeric_limits<int>::max();
    Entity player;
    player.hp = 100;
    for (const auto& weapon : weapons) {
        for (const auto& armour : armours) {
            {
                // No rings
                player.damage = weapon.damage;
                player.armour = armour.armour;
                if (calculate_strikes(player, enemy) <= calculate_strikes(enemy, player)) {
                    int cost = weapon.cost + armour.cost;
                    min_cost = std::min(cost, min_cost);
                } 
            }
            for (int r1 = 0; r1 < rings.size(); r1++) {   
                for (int r2 = r1+1; r2 < rings.size(); r2++) {
                    player.damage = weapon.damage;
                    player.armour = armour.armour;
                    player.damage += rings[r1].damage + rings[r2].damage;
                    player.armour += rings[r1].armour + rings[r2].armour;
                    if (calculate_strikes(player, enemy) <= calculate_strikes(enemy, player)) {
                        int cost = weapon.cost + armour.cost + rings[r1].cost + rings[r2].cost;
                        min_cost = std::min(cost, min_cost);
                    }
                }
            }
        }
    }
    std::cout << min_cost << '\n';
}