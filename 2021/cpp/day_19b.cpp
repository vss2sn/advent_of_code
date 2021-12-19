#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert>

#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

struct Point {
  int x, y, z;
  Point(const int x, const int y, const int z) : x(x), y(y), z(z) {}
  bool operator == (const Point& p) const {
    return x == p.x && y == p.y && z == p.z;
  }

  Point operator + (const Point& p) const {
    return Point(x + p.x, y + p.y, z + p.z);
  }

  Point operator - (const Point& p) const {
    return Point(x - p.x, y - p.y, z - p.z);
  }

  bool operator < (const Point& p) {
    return (z < p.z) ||
      (z == p.z && y < p.y) ||
      (z == p.z && y == p.y && x < p.x);
  }
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
  return os;
}

struct hash_point {
  std::size_t operator () (const Point& p) const {
    return p.z * 1000 * 1000 + p.y * 1000 + p.x;
  }
};

void print(const std::vector<Point>& v) {
  for (const auto& ele : v) {
    std::cout << ele << '\n';
  }
  std::cout << '\n';
}

using FT = Point(const Point&);

const auto transforms = std::vector<FT*>{
  [](const Point& p) { return Point(p.x, p.y, p.z); },
  [](const Point& p) { return Point(p.x, -p.z, p.y); },
  [](const Point& p) { return Point(p.x, -p.y, -p.z); },
  [](const Point& p) { return Point(p.x, p.z, -p.y); },
  [](const Point& p) { return Point(-p.y, p.x, p.z); },
  [](const Point& p) { return Point(p.z, p.x, p.y); },
  [](const Point& p) { return Point(p.y, p.x, -p.z); },
  [](const Point& p) { return Point(-p.z, p.x, -p.y); },
  [](const Point& p) { return Point(-p.x, -p.y, p.z); },
  [](const Point& p) { return Point(-p.x, -p.z, -p.y); },
  [](const Point& p) { return Point(-p.x, p.y, -p.z); },
  [](const Point& p) { return Point(-p.x, p.z, p.y); },
  [](const Point& p) { return Point(p.y, -p.x, p.z); },
  [](const Point& p) { return Point(p.z, -p.x, -p.y); },
  [](const Point& p) { return Point(-p.y, -p.x, -p.z); },
  [](const Point& p) { return Point(-p.z, -p.x, p.y); },
  [](const Point& p) { return Point(-p.z, p.y, p.x); },
  [](const Point& p) { return Point(p.y, p.z, p.x); },
  [](const Point& p) { return Point(p.z, -p.y, p.x); },
  [](const Point& p) { return Point(-p.y,-p.z, p.x); },
  [](const Point& p) { return Point(-p.z, -p.y, -p.x); },
  [](const Point& p) { return Point(-p.y, p.z, -p.x); },
  [](const Point& p) { return Point(p.z, p.y, -p.x); },
  [](const Point& p) { return Point(p.y, -p.z, -p.x); }

};

const auto inverse_transforms = std::vector<FT*> {
  [](const Point& p) { return Point(p.x, p.y, p.z); },//1
  [](const Point& p) { return Point(p.x, p.z, -p.y); },//2
  [](const Point& p) { return Point(p.x, -p.y, -p.z); },//3
  [](const Point& p) { return Point(p.x, -p.z, p.y); },//4
  [](const Point& p) { return Point(p.y, -p.x, p.z); },//5
  [](const Point& p) { return Point(p.y, p.z, p.x); },//6
  [](const Point& p) { return Point(p.y, p.x, -p.z); },//7
  [](const Point& p) { return Point(p.y, -p.z, -p.x); },//8
  [](const Point& p) { return Point(-p.x, -p.y, p.z); },//9
  [](const Point& p) { return Point(-p.x, -p.z, -p.y); },//10
  [](const Point& p) { return Point(-p.x, p.y, -p.z); },//11
  [](const Point& p) { return Point(-p.x, p.z, p.y); },//12
  [](const Point& p) { return Point(-p.y, p.x, p.z); },//13
  [](const Point& p) { return Point(-p.y,-p.z, p.x); },//14
  [](const Point& p) { return Point(-p.y, -p.x, -p.z); },//15
  [](const Point& p) { return Point(-p.y, p.z, -p.x); },//16
  [](const Point& p) { return Point(p.z, p.y, -p.x); },//17
  [](const Point& p) { return Point(p.z, p.x, p.y); },//18
  [](const Point& p) { return Point(p.z, -p.y, p.x); },//19
  [](const Point& p) { return Point(p.z, -p.x, -p.y); },//20
  [](const Point& p) { return Point(-p.z, -p.y, -p.x); },//21
  [](const Point& p) { return Point(-p.z, -p.x, p.y); },//22
  [](const Point& p) { return Point(-p.z, p.y, p.x); },//23
  [](const Point& p) { return Point(-p.z, p.x, -p.y); }// 24
};

struct Scanner {
  std::vector<Point> input_beacons;
  std::vector<Point> vectors_between_beacons_at_input_orientation;
  std::vector<std::unordered_set<Point, hash_point>> vectors_between_beacons;
  std::vector<Point> beacon_coordinates_in_global_frame;
  int chosen_orienation = -1; // the index of the transform in transforms used to convert from scanner frame to global frame
  int id = -1;
  Point coordinates = Point(0,0,0);
};

Point get_first_point_of_common_vector_in_global_frame(const Scanner& s, const Point& common_vector_between_points) {
  // Convert the common_vector (which is the vector between 2 points in the scanner frame and the orientation 's.chosen_orienation') to
  // the original scanner frame (the one in which the input is given)
  auto original_common_vector_between_points = inverse_transforms[s.chosen_orienation](common_vector_between_points);
  int index1 = -1, index2 = -1;
  bool found = false;
  // Find the indices of the 2 points that were used to create the common_vector from the input
  for (int i = 0; i < s.input_beacons.size(); i++) {
    for (int j = i+1; j < s.input_beacons.size(); j++) {
      const auto delta = Point(
        s.input_beacons[i].x - s.input_beacons[j].x,
        s.input_beacons[i].y - s.input_beacons[j].y,
        s.input_beacons[i].z - s.input_beacons[j].z
      );
      if (delta == original_common_vector_between_points) {
        found= true;
        index1 = i;
        index2 = j;
        break;
      }
      if (found) break;
    }
  }
  // Sanity check
  assert(index1 != -1);
  assert(index2 != -1);
  // Convert the point into the global frame using the chosen orientation
  return transforms[s.chosen_orienation](s.input_beacons[index1]);
}


void calculate_s2_coordinate(const Scanner& s1, Scanner& s2, const Point& common_vector_between_points) {
  const auto p1 = get_first_point_of_common_vector_in_global_frame(s1, common_vector_between_points);
  const auto p2 = get_first_point_of_common_vector_in_global_frame(s2, common_vector_between_points);
  s2.coordinates = s1.coordinates + p1 - p2;
}

int main(int argc, char * argv[]) {

  // Sanity check on the transforms and inverse transforms
  // const auto p = Point(1,2,3);
  // std::unordered_set<Point, hash_point>  temp_s;
  // for (int i = 0; i < transforms.size(); i++) {
  //   assert(p == inverse_transforms[i](transforms[i](p)));
  //   assert(temp_s.find(transforms[i](p)) == temp_s.end());
  // }

  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Scanner> scanners;
  while(std::getline(file, line)) {
    scanners.emplace_back();
    auto& s = scanners.back();
    s.id = line[12] - '0';
    while(std::getline(file, line)) {
      if (line.empty()) break;
      const auto dlim_i_1 = line. find(',');
      const auto dlim_i_2 = line. find(',', dlim_i_1+1);
      s.input_beacons.emplace_back(
        std::stoi(line.substr(0, dlim_i_1)),
        std::stoi(line.substr(dlim_i_1 + 1, dlim_i_2 - dlim_i_1)),
        std::stoi(line.substr(dlim_i_2 + 1, line.size() - dlim_i_2))
      );
    }
  }

  for (auto& s : scanners) {
    // Calculate the relative positions of beasons from each other
    for (int i = 0; i < s.input_beacons.size(); i++) {
      for (int j = i+1; j < s.input_beacons.size(); j++) {
        s.vectors_between_beacons_at_input_orientation.emplace_back(
          s.input_beacons[i].x - s.input_beacons[j].x,
          s.input_beacons[i].y - s.input_beacons[j].y,
          s.input_beacons[i].z - s.input_beacons[j].z);
      }
    }

    // Convert the relative positions into differnet orientations
    for (const auto& transform : transforms) {
      s.vectors_between_beacons.emplace_back();
      auto& beacon_at_orinetation_s = s.vectors_between_beacons.back();
      for (const auto& p : s.vectors_between_beacons_at_input_orientation) {
        const auto temp = transform(p);
        beacon_at_orinetation_s.insert(transform(p));
      }
    }
  }

  // Check for > 12 overlap
  constexpr int n_overlap = 12;
  scanners[0].chosen_orienation = 0;
  std::queue<int> scanners_already_matched_q;
  std::unordered_set<int> scanners_already_matched_us;
  scanners_already_matched_q.push(0);
  scanners_already_matched_us.insert(0);

  while(!scanners_already_matched_q.empty()) {
    auto& s1 = scanners[scanners_already_matched_q.front()];
    // std::cout << "Popped scanner " << s1.id << '\n';
    scanners_already_matched_q.pop();
    for (int i = 0; i < scanners.size(); i++) {
      // std::cout << "Checking against scanner " << i << '\n';
      if (i == s1.id) {
        // std::cout << "Skip checking against itself " << '\n';
        continue;
      }
      if (scanners_already_matched_us.find(i) != scanners_already_matched_us.end()) {
        // std::cout << "Scanner " << i << " has already been positioned. Skipping. " << '\n';
        continue;
      }
      auto& s2 = scanners[i];
      bool found_orientation = false;
      for (int i_orient = 0; i_orient < 24; i_orient++) {
        // std::this_thread::sleep_for(100ms);
        // std::cout << "orientation: " << i_orient << '\n';
        int count = 0;
        for (const auto& ele : s2.vectors_between_beacons[i_orient]) {
          if (s1.vectors_between_beacons[s1.chosen_orienation].find(ele) != s1.vectors_between_beacons[s1.chosen_orienation].end()) {
            count++;
          }
          if (count >= 12) {
            found_orientation = true;
            // std::cout << "Scanner " << i <<" matches scanner "<< s1.id << '\n';
            s2.chosen_orienation = i_orient;
            calculate_s2_coordinate(s1, s2, ele);
            scanners_already_matched_q.push(i);
            scanners_already_matched_us.insert(i);
            break;
          }
          if (found_orientation) break;
        }
      }
    }
  }

  auto distance = std::numeric_limits<int>::min();
  for (int i = 0; i < scanners.size(); i++) {
    for (int j = i + 1; j < scanners.size(); j++) {
      auto d = scanners[i].coordinates - scanners[j].coordinates;
      distance = std::max(distance, std::abs(d.x)+std::abs(d.y)+std::abs(d.z));
    }
  }

  std::cout<< distance << '\n';
  return 0;
}
