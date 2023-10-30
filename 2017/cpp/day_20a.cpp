#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>

struct Coord3D {
  int x;
  int y;
  int z;

  int dist () const {
    return std::abs(x) + std::abs(y) + std::abs(z);
  }

  int sum () const {
    return x + y + z;
  }

  Coord3D operator + (const Coord3D& c) {
    Coord3D ans;
    ans.x = x + c.x;
    ans.y = y + c.y;
    ans.z = z + c.z;
    return ans;
  }

  Coord3D operator += (const Coord3D& c) {
    x += c.x;
    y += c.y;
    z += c.z;
    return *this;
  }
};

int dot_product(const Coord3D& v0, const Coord3D& v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

struct Particle {
  Coord3D p;
  Coord3D v;
  Coord3D a;
};

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_20_input" ;  
  std::ifstream file(input);
  std::string line;
  const std::regex pattern(R"(p=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, v=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, a=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>)");
  std::vector<Particle> particles;
  int min_acc = std::numeric_limits<int>::max();
  std::vector<int> particles_with_min_acc;
  // Find all particles with the minimum acceleration 
  // as at t-> infinity, those will be the closest
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    Particle particle;
    particle.p.x = std::stoi(match[1]);
    particle.p.y = std::stoi(match[2]);
    particle.p.z = std::stoi(match[3]);
    particle.v.x = std::stoi(match[4]);
    particle.v.y = std::stoi(match[5]);
    particle.v.z = std::stoi(match[6]);
    particle.a.x = std::stoi(match[7]);
    particle.a.y = std::stoi(match[8]);
    particle.a.z = std::stoi(match[9]);
    particles.push_back(particle);
    if (particle.a.dist() < min_acc) {
      particles_with_min_acc.clear();
      particles_with_min_acc.push_back(particles.size()-1);
      min_acc = particle.a.dist();
    } else if (particle.a.dist() == min_acc) {
      particles_with_min_acc.push_back(particles.size()-1);
    }
  }

  // If particles have the same acceleration, 
  // Find the time at which each is closest to the origin
  // Get the latest time 
  // Check the other particles' distances
  // Lowest distance at the final time 

  int ans = particles_with_min_acc[0];
  int min_dist = std::numeric_limits<int>::max();
  int time = 0;
  while (std::any_of(
            std::begin(particles_with_min_acc), 
            std::end(particles_with_min_acc), 
            [&particles](const int idx) {
              const auto& particle = particles[idx];
              return dot_product(particle.p, particle.v) < 0 || dot_product(particle.p, particle.a) < 0;
            })) {
    for (const auto idx : particles_with_min_acc) {
      auto& particle = particles[idx];
      if (min_dist > particle.p.dist()) {
          // std::cout << idx << " is closest at time " << time << '\n';
          min_dist = particle.p.dist();
          ans = idx;      
      }
      particle.p = particle.p + particle.v;
      particle.v = particle.v + particle.a;
    }
    time++;
  }
  std::cout << ans << '\n';
  return 0;
}
