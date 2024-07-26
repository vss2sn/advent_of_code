#include <fstream>
#include <iostream>
#include <string> 
#include <unordered_set> 

struct Point {
    int row;
    int col;

    bool operator == (const Point& p) const {
        return p.row ==row && p.col == col;
    }
};

struct PointHasher {
    std::size_t operator () (const Point& p) const  {
        return (p.row << 10) + p.col;
    }
};

void updatePoint(Point& current, const char dir) {
    if (dir == '<') {
        current.col--;
    } else if (dir == '>') {
        current.col++;
    } else  if (dir == '^') {
        current.row--;
    } else if (dir == 'v') {
        current.row++;
    } else {
        std::cout << "This should not happen";
        std::cout << "Unknown symbol: " << dir << '\n';
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_03_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::ifstream file(input);
    while (std::getline(file, line)) { // to allow for sample input
        std::unordered_set<Point, PointHasher> seen; 
        std::array<Point, 2> currents = {{Point(), Point()}};
        currents[0].row = 0;
        currents[0].col = 0;
        currents[1].row = 0;
        currents[1].col = 0;
        seen.insert(currents[0]);
        bool original_santa = true;
        for(const auto ele : line) {
            if (original_santa) {
                updatePoint(currents[0], ele);
                seen.insert(currents[0]);
            } else {
                updatePoint(currents[1], ele);
                seen.insert(currents[1]);
            }
            original_santa = !original_santa;
        }
        std::cout << seen.size() << '\n';
    }
    return 0;
}