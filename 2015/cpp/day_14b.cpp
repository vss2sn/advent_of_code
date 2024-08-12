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
    bool is_resting;
    int timeout;
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
    std::vector<int> points(reindeer.size(), 0);
    std::vector<int> distances(reindeer.size(), 0);

    for (auto& r : reindeer) {
        r.timeout = r.time_of_flight;
        r.is_resting = false;
    }
    for (int time = 0; time < race_time; time++) {
        for (int i = 0; i < reindeer.size(); i++) {
            auto& r = reindeer[i];
            r.timeout--;
            if (!r.is_resting) {
                distances[i] += r.speed;
            }
            if (r.timeout == 0) {
                r.is_resting = !r.is_resting;
                r.timeout = r.is_resting ? r.time_of_rest : r.time_of_flight;
            }
        }
        const int leading_pts = *std::max_element(std::begin(distances), std::end(distances));
        for (int i = 0; i < distances.size(); i++) {
            if (distances[i] == leading_pts) {
                points[i]++;
            }
        }
    }
    const auto winner_idx = std::distance(std::begin(points), std::max_element(std::begin(points), std::end(points)));
    std::cout <<  points[winner_idx] << '\n';

    return 0;
}