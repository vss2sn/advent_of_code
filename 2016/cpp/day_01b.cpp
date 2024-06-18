#include <fstream>
#include <iostream>
#include <unordered_set> 
#include <string>

struct Point {
    int x;
    int y;

    bool operator == (const Point& p) const {
        return p.x == x && p.y == y;
    }
};

struct hasher {
    std::size_t operator () (const Point& p) const {
        return p.x * 10000 + p.y;
    }
};

bool move (const std::size_t& start, const std::size_t& end, Point& p, int& direction, const std::string& instructions, std::unordered_set<Point, hasher>& visited) {
    if (instructions[start] == 'L') {
        direction--;
    } else {
        direction++;
    }
    if(direction < 0) {
        direction += 4;
    }
    direction %= 4;
    // Instead of using substr and then stoi can use this:
    int num = 0;
    for (std::size_t idx = start+1; idx < end; idx++) {
        num = num * 10 + (instructions[idx] - '0');
    }
    for(int i = 0; i < num; i++) {
        if (direction == 0) {
            p.y+=1;
        }
        else if (direction == 1) {
            p.x+=1;
        }
        else if (direction == 2) {
            p.y-=1;
        }
        else  {
            p.x-=1;
        }
        const auto [it, inserted] = visited.insert(p);
        if (!inserted) {
           std::cout << std::abs(p.x) + std::abs(p.y) << '\n';
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string instructions;
  std::getline(file, instructions);
  Point p;
  p.x = 0;
  p.y = 0;
  int direction = 0;
  std::unordered_set<Point, hasher> visited;
  visited.insert(p);
  std::size_t start = 0;
  const std::string delimiter = ", ";
  std::size_t end = instructions.find(delimiter, start);
  while(end != std::string::npos) {
    if(move(start, end, p, direction, instructions, visited)) {
        return 0;
    }
    start = end + delimiter.size();
    end = instructions.find(delimiter, start);    
  }
  move(start, instructions.size(), p, direction, instructions, visited);
  // Question guarantees that there will be at least 1 point revisited.
  return 0;
}
