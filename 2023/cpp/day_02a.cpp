#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Use a regex?
std::array<int, 3> parse_round(const std::string& round_str) {
  size_t start = 0;
  std::size_t end = round_str.find(',');
  std::array<int,3> round {{0,0,0}};
  while (end != std::string::npos) {
    const std::string colour_data = round_str.substr(start, end - start);
    const auto i = colour_data.find(' ');
    const auto colour = colour_data.substr(i + 1, colour_data.size() - i - 1);
    const auto n = std::stoi(colour_data.substr(0, i));
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
    start = end + 2;
    end = round_str.find(',', start);
  }
  const std::string colour_data = round_str.substr(start, round_str.size() - start);
  const auto i = colour_data.find(' ');
  const auto colour = colour_data.substr(i + 1, colour_data.size() - i - 1);
  const int n = std::stoi(colour_data.substr(0, i));
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
  return round;
}

struct Game {
  int id;
  std::vector<std::array<int, 3>> rounds;
};  

Game parse_game(const std::string& game_str) {
  size_t start = 0;
  std::size_t end = game_str.find(':');
  const int game_number = std::stoi(game_str.substr(5, end - 5));
  start = end + 1;
  end = game_str.find(';', start);
  Game game;
  game.id = game_number;
  while (end != std::string::npos) {
    const std::string round_data = game_str.substr(start + 1, end - start - 1);
    game.rounds.push_back(parse_round(round_data));
    start = end + 1;
    end = game_str.find( ';', start);
  }
  const std::string round_data = game_str.substr(start + 1, game_str.size() - start - 1);
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

  int count = 0;
  for (const auto& game : games) {
    bool playable = true;
    for (const auto& round : game.rounds) {
      if (round[0] > 12 || round[1] > 13 || round[2] > 14) {
        playable = false;
        break;
      }
    }
    if (playable) {
      count += game.id;
    }
  } 
  std::cout << count << '\n';
  return 0;
}
