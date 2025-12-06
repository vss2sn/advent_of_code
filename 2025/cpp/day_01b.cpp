#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string input = "../input/day_01_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    int position = 50;
    int count = 0; 
    while(std::getline(file, line)) {
        const int value = std::stoi(line.substr(1, line.size() - 1));
        const int mod_value = value % 100;
        count += value / 100;
        if (line[0] == 'R') {
            position += mod_value;
            if (position > 99) {
                count++;
                position = position - 100;
            }
        }
        else {
            const int old_position = position;
            position -= mod_value;
            if (position < 0) {
                if (old_position != 0) count++;
                position = position + 100;
            }
            if (position == 0) count++;
        }
    }
    std::cout << count << '\n';
    return 0;
}