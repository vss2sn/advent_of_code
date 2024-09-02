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

// Read question as effect only occurs on the turn of a player/boss not on both

struct Stat {
    int perm = 0;
    int temp = 0;
    int time_left = 0;
};

enum Effects {
    Shield,
    Poison,
    Recharge,
    None,
};

struct Entity { 
    Stat hp;
    Stat damage;
    Stat armour;
    Stat mana;
    std::unordered_set<Effects> effects;
};

struct Spell {
    std::string name;
    int cost;
    Effects effect;
    std::function<void(Entity&, Entity&)> function;
};

std::vector<Spell> spells {
    {"Magic Missile", 53, Effects::None, [](Entity& e1, Entity& e2) {e1.mana.perm -= 53; e2.hp.perm -= 4;}},
    {"Drain" , 73 , Effects::None, [](Entity& e1, Entity& e2) {e1.mana.perm -= 73; e1.hp.perm += 2; e2.hp.perm -= 2;}},
    {"Shield" , 113, Effects::Shield, [](Entity& e1, Entity& e2) { e1.mana.perm -= 113; e1.armour.temp = 7; e1.armour.time_left = 6; e1.effects.insert(Effects::Shield);}},
    {"Poison" , 173 , Effects::Poison, [](Entity& e1, Entity& e2) { e1.mana.perm -= 173; e2.hp.temp = -3; e2.hp.time_left = 6; e2.effects.insert(Effects::Poison);}},
    {"Recharge" , 229, Effects::Recharge, [](Entity& e1, Entity& e2) { e1.mana.perm -= 229; e1.mana.temp = 101; e1.mana.time_left = 5; e1.effects.insert(Effects::Recharge);}},
    // {"None" , 0, [](Entity& e1, Entity& e2) { }},
};

int min_mana_spent = std::numeric_limits<int>::max();

void resolve_effects (Entity& e) {
    e.hp.perm += e.hp.temp;
    e.mana.perm += e.mana.temp;
    if (e.hp.time_left > 0) e.hp.time_left--;
    if (e.damage.time_left > 0) e.damage.time_left--;
    if (e.armour.time_left > 0) e.armour.time_left--;
    if (e.mana.time_left > 0) e.mana.time_left--;
    if (e.hp.time_left == 0) {
        e.effects.erase(Effects::Poison);
    }
    if (e.armour.time_left == 0) {
        e.effects.erase(Effects::Shield);
    }
    if (e.mana.time_left == 0) {
        e.effects.erase(Effects::Recharge);
    }
}

void reset_timed_out_effects(Entity& e) {
    if (e.hp.time_left == 0) {
        e.hp.temp = 0;
    }
    if (e.damage.time_left == 0) {
        e.damage.temp = 0;
    }
    if (e.armour.time_left == 0) {
        e.armour.temp = 0;
    }
    if (e.mana.time_left == 0) {
        e.mana.temp = 0;
    }
}

bool should_return(const Entity& player_in, const Entity& boss_in, int mana_spent) {
    if (boss_in.hp.perm <= 0) {
        min_mana_spent = std::min(mana_spent, min_mana_spent); 
        return true;
    }
    if (player_in.hp.perm <= 0) {
        return true;
    }
    return false;
}

void fight(const Entity& player_in, const Entity& boss_in, bool player_turn, int mana_spent, int level) {
    if (mana_spent >= min_mana_spent) return;
    if (should_return(player_in, boss_in, mana_spent)) return;
    if (player_turn) {
        for(const auto& spell : spells) {
            if (player_in.mana.perm < spell.cost) continue;
            auto player = player_in;
            auto boss = boss_in;
            player.hp.perm--;
            if (should_return(player_in, boss_in, mana_spent)) return;
            resolve_effects(player);
            resolve_effects(boss);
            if (player.effects.count(spell.effect)) continue;
            if (boss.effects.count(spell.effect)) continue;
            if (should_return(player_in, boss_in, mana_spent)) return;
            spell.function(player, boss);
            reset_timed_out_effects(player);
            reset_timed_out_effects(boss);
            fight(player, boss, false, mana_spent + spell.cost, level+1);
        }
    } else {
        auto player = player_in;
        auto boss = boss_in;
        resolve_effects(player);
        resolve_effects(boss);
        if (should_return(player_in, boss_in, mana_spent)) return;
        player.hp.perm -= std::max(1, boss.damage.perm - player.armour.perm - player.armour.temp);
        reset_timed_out_effects(boss);
        reset_timed_out_effects(player);
        fight(player, boss, true, mana_spent, level+1);
    }
}

int main(int argc, char * argv[]) {
    std::string input = "../input/day_22_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::getline(file, line);
    Entity enemy;
    enemy.hp.perm = std::stoi(line.substr(line.find(':') + 2, line.size()- line.find(':') - 2));
    std::getline(file, line);
    enemy.damage.perm = std::stoi(line.substr(line.find(':') + 2, line.size()- line.find(':') - 2));
    Entity player;
    player.hp.perm = 50;
    player.mana.perm = 500;
    
    // // 10 hit points and 250 mana, and that the boss has 13 hit points and 8
    // player.hp.perm = 10;
    // player.mana.perm = 250;
    // enemy.hp.perm = 13;
    // enemy.damage.perm = 8;
    
    // // 10 hit points and 250 mana, and that the boss has 14 hit points and 8
    // player.hp.perm = 10;
    // player.mana.perm = 250;
    // enemy.hp.perm = 14;
    // enemy.damage.perm = 8;

    fight(player, enemy, true, 0, 1);
    std::cout << min_mana_spent << '\n';
}