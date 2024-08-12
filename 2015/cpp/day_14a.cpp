#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex> 
#include <string>
#include <vector>

// Update the time as necessary based on the input
// constexpr int race_time = 1000;
constexpr int race_time = 2503;

struct Reindeer {
    std::string name;
    int speed;
    int time_of_flight;
    int time_of_rest;
};

int calc_dist(const Reindeer& r) {
    int time = 0;
    int dist = 0;
    while(time < race_time) {
        const auto flight_time = std::min(r.time_of_flight, race_time - time);
        dist += r.speed * flight_time;
        time += flight_time;
        const auto rest_time = std::min(r.time_of_rest, race_time - time);
        time += rest_time;
    }
    return dist;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_14_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::regex pattern(R"(([A-Za-z]+) can fly ([0-9]+) km/s for ([0-9]+) seconds, but then must rest for ([0-9]+) seconds.)");
    std::vector<Reindeer> reindeer;
    while(std::getline(file, line)) {
        std::smatch match;
        std::regex_search(line, match, pattern);
        Reindeer r;
        r.name = match[1];
        r.speed = std::stoi(match[2]);
        r.time_of_flight =std::stoi( match[3]);
        r.time_of_rest = std::stoi(match[4]);
        reindeer.push_back(r);
    }
    std::vector<int> distances(reindeer.size(), 0);
    for (int i = 0; i < reindeer.size(); i++) {
        distances[i] = calc_dist(reindeer[i]);
    }
    std::cout << *std::max_element(std::begin(distances), std::end(distances)) << '\n';
    return 0;
}