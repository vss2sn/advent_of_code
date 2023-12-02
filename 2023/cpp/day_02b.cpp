#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Use a regex?
void update_colour(std::array<int, 3>& round, const std::string_view colour_data) {
  const auto i = colour_data.find(' ');
  const auto colour = colour_data.substr(i + 1, colour_data.size() - i - 1);
  int n = 0;
  for (const auto ele : colour_data.substr(0, i)) {
    n = n * 10 + (ele - '0');
  }
  if (colour == "red") {
    round[0] = n;
  } else if (colour == "green") {
    round[1] = n;
  } else if (colour =="blue") {
    round[2] = n;
  } else {
    std::cout << "This should not happen" << __LINE__ << '\n';
    exit(0);
  }
}

std::array<int, 3> parse_round(const std::string_view round_str) {
  std::size_t start = 0;
  std::size_t end = round_str.find(',');
  std::array<int,3> round {{0,0,0}};
  while (end != std::string::npos) {
    const std::string_view colour_data = round_str.substr(start, end - start);
    update_colour(round, colour_data);
    start = end + 2;
    end = round_str.find(',', start);
  }
  const std::string_view colour_data = round_str.substr(start, round_str.size() - start);
  update_colour(round, colour_data);
  return round;
}

struct Game {
  int id;
  std::vector<std::array<int, 3>> rounds;
};  

Game parse_game(const std::string_view game_str) {
  std::size_t start = 0;
  std::size_t end = game_str.find(':');
  int game_number = 0;
  for (const auto ele : game_str.substr(5, end - 5)) {
    game_number = game_number * 10 + (ele - '0');
  }
  start = end + 1;
  end = game_str.find(';', start);
  Game game;
  game.id = game_number;
  while (end != std::string::npos) {
    const std::string_view round_data = game_str.substr(start + 1, end - start - 1);
    game.rounds.push_back(parse_round(round_data));
    start = end + 1;
    end = game_str.find( ';', start);
  }
  const std::string_view round_data = game_str.substr(start + 1, game_str.size() - start - 1);
  game.rounds.push_back(parse_round(round_data));
  return game;
}


int main(int argc, char * argv[]) {
  std::string input = "../input/day_02_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Game> games;
  while(std::getline(file, line)) {
    games.emplace_back(parse_game(line));
  }

  std::size_t total = 0;
  for (const auto& game : games) {
    std::array<int, 3> min_cubes {{0,0,0}};  
    for (const auto& round : game.rounds) {
      for (int i = 0; i < 3; i++) {
        min_cubes[i] = std::max(min_cubes[i], round[i]);
      }
    }
    total  += min_cubes[0] * min_cubes[1] * min_cubes[2];
  } 
  std::cout << total << '\n';
  return 0;
}
