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

int main(int argc, char* argv[]) {
    std::string input = "../input/day_03_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::ifstream file(input);
    while (std::getline(file, line)) { // to allow for sample input
        std::unordered_set<Point, PointHasher> seen; 
        Point current;
        current.row = 0;
        current.col = 0;
        seen.insert(current);
        for(const auto ele : line) {
            if (ele == '<') {
                current.col--;
            } else if (ele == '>') {
                current.col++;
            } else  if (ele == '^') {
                current.row--;
            } else if (ele == 'v') {
                current.row++;
            } else {
                std::cout << "This should not happen";
                std::cout << "Unknown symbol: " << ele << '\n';
                exit(0);
            }
            seen.insert(current);
        }
        std::cout << seen.size() << '\n';
    }
    return 0;
}