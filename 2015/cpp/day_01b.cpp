#include <fstream>
#include <iostream>
#include <string> 

int main(int argc, char* argv[]) {
    std::string input = "../input/day_01_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::ifstream file(input);
    while (std::getline(file, line)) { // to allow for sample input
        int floor = 0;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '(') {
                floor++;
            } else if(line[i] == ')') {
                floor--;
            }
            if (floor == -1) {
                std::cout << i + 1 << '\n'; // index starts at 1
                return 0;
            }
        }
    }
    return 0;
}