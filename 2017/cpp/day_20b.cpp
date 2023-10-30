#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <regex>

// Note: increment velocity first, then position (mentioned in part 1 i.e. part a) 
// time: position(time)
// 0: p(0) = p0
// 1: p(1) = p0 + v + a
// 2: p(2) = p0 + v + a + v + 2a
// n: p(n) = p0 + v + a + ... + v + na = p0 + n * v + n(n+1)/2 * a
// p(t) = (a * t^2 + (a + 2v) * t + 2 * p0) / 2

struct Coord3D {
  long long x;
  long long y;
  long long z;

  long long dist () const {
    return std::abs(x) + std::abs(y) + std::abs(z);
  }

  long long sum () const {
    return x + y + z;
  }

  Coord3D operator + (const Coord3D& c) const {
    Coord3D ans;
    ans.x = x + c.x;
    ans.y = y + c.y;
    ans.z = z + c.z;
    return ans;
  }
  
  Coord3D operator - (const Coord3D& c) const {
    Coord3D ans;
    ans.x = x - c.x;
    ans.y = y - c.y;
    ans.z = z - c.z;
    return ans;
  }

  Coord3D operator += (const Coord3D& c) {
    x += c.x;
    y += c.y;
    z += c.z;
    return *this;
  }
};

long long dot_product(const Coord3D& v0, const Coord3D& v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

struct Particle {
  Coord3D p;
  Coord3D v;
  Coord3D a;
};

struct Collision {
  long long id0;
  long long id1;
  long long time;
};

int find_position_at_time (const int p, const int v, const int a, const int time) {
	// std::cout << "Values: " << s << ' ' << v << ' ' << a << ' ' << time << '\n';
	// std::cout << "Position: " << s + (time + 1 ) * v + (time * time + 1 * a) / 2 << '\n';
	return (a * time * time + (a + 2 * v) * time + 2 * p) / 2;
}

std::vector<long long> find_time_of_potential_collision(const int p0, const int p1, const int v0, const int v1, const int a0, const int a1) {
	const auto a = float(a1 - a0);
	float t0 = 0;
	float t1 = 0;
	if (a == 0) {
		t0 = -float(p1 -p0) / float(v1 - v0);
		t1 = -1;
	} else {
		const auto b = float((a1 - a0) + 2 * (v1 - v0));
		const auto c = float(2.f * (p1 - p0));
		// std::cout << "Quad equation values: " << a << ' ' << b << ' ' << c << '\n'; 
		const auto discriminent_2 = (b * b) - 4 * a *c;
		if (discriminent_2 < 0) {
			// std::cout << "No collision possible; negative b2-4ac" << '\n';
			return {-1, -1};
		}
    // TODO: improve this check
		const auto discriminent = std::sqrt(discriminent_2);
		if (discriminent != int(discriminent)) {
			// std::cout << "No collision possible, not a perfect square: " << discriminent << '\n';
			return {-1, -1};
		}
	  t0 = (-b + discriminent) / (2 * a);
	  t1 = (-b - discriminent) / (2 * a);
	}
	if (t0 < 0) t0 = -1;
	if (t1 < 0) t1 = -1;
	// std::cout << t0 << ' ' << t1 << '\n';
	if (t0 != int(t0)) t0 = -1;
	if (t1 != int(t1)) t1 = -1;
	// std::cout << "Possible times: " << t0 << ' ' << t1 << '\n';
	return {static_cast<long long>(t0), static_cast<long long>(t1)};
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_20_input" ;  
  std::ifstream file(input);
  std::string line;
  const std::regex pattern(R"(p=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, v=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, a=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>)");
  std::vector<Particle> particles;
  long long min_acc = std::numeric_limits<long long>::max();
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
  }

  std::vector<Collision> collisions;
	for (int i = 0; i < particles.size(); i++) {
		for (int j = i + 1; j < particles.size(); j++) {
			// std::cout << "\nPair: (" << i << "," << j << "): " << '\n';
			const auto times = find_time_of_potential_collision(particles[i].p.x, particles[j].p.x, particles[i].v.x, particles[j].v.x, particles[i].a.x, particles[j].a.x);
			for (const auto time : times) {
				// std::cout << "Time: " << time << '\n';
				if (time != -1 && find_position_at_time(particles[i].p.y, particles[i].v.y, particles[i].a.y, time)
						== find_position_at_time(particles[j].p.y, particles[j].v.y, particles[j].a.y, time) &&
						find_position_at_time(particles[i].p.z, particles[i].v.z, particles[i].a.z, time)
						== find_position_at_time(particles[j].p.z, particles[j].v.z, particles[j].a.z, time) ) {
							// std::cout << "Collision!!!!!!!!!! Time: " << time << '\n';
              Collision c;
              c.id0 = i;
              c.id1 = j;
              c.time = time;
              collisions.push_back(c);
				} 
			}
		}
	}

  std::sort(std::begin(collisions), std::end(collisions), [](const auto& c1, const auto& c2) {return c1.time < c2.time; });
  // for (const auto & c : collisions) {
  //   std::cout << c.time << '\n';
  // }
  std::unordered_map<long long, long long> id_to_destruction_time;
  for (const auto& c : collisions) {
    if (id_to_destruction_time.find(c.id0) == id_to_destruction_time.end() && id_to_destruction_time.find(c.id1) == id_to_destruction_time.end()) {
      id_to_destruction_time[c.id0] = c.time;
      id_to_destruction_time[c.id1] = c.time;
      // std::cout << "Adding " << c.id0 << '\n';
      // std::cout << "Adding " << c.id1 << '\n';
    } else if (id_to_destruction_time.find(c.id0) != id_to_destruction_time.end() && id_to_destruction_time[c.id0] == c.time) {
      id_to_destruction_time[c.id1] = c.time;
      // std::cout << "Adding " << c.id1 << '\n';
    } else if (id_to_destruction_time.find(c.id1) != id_to_destruction_time.end() && id_to_destruction_time[c.id1] == c.time) {
      id_to_destruction_time[c.id0] = c.time;
      // std::cout << "Adding " <<   c.id0 << '\n';
    }
  } 
	std::cout << particles.size() - id_to_destruction_time.size() << '\n'; 
  return 0;
}


