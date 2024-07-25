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
        for (const auto ele : line) {
            if (ele == '(') {
                floor++;
            } else if(ele == ')') {
                floor--;
            }
        }
        std::cout << floor << '\n';
    }
    return 0;
}