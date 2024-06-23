#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <unordered_set>
#include <cassert>  

constexpr std::size_t rows = 6;
constexpr std::size_t cols = 50;
enum class Instruction{
    RECT, ROTATE_ROW, ROTATE_COL
};

struct Parsed {
    Instruction instr;
    int dim_1;
    int dim_2;
};

Parsed parse_input(const std::string& line) {
    Parsed p;
    if (line[1] == 'e') {
        const auto idx = line.find('x');
        p.instr = Instruction::RECT;
        // std::cout << '|' << line.substr(5, idx - 5) << '|' <<'\n';
        p.dim_1 = std::stoi(line.substr(5, idx - 5));
        // std::cout << '|' << line.substr(idx+1, line.size() - idx) << '|' <<'\n';
        p.dim_2 = std::stoi(line.substr(idx+1, line.size() - idx));
    } else {
        if (line[7] == 'r') {
            p.instr = Instruction::ROTATE_ROW;    
        } else if (line[7] == 'c'){
            p.instr = Instruction::ROTATE_COL;
        } else { 
            std::cout << "This should not happen" << '\n';
            exit(00);
        }
        const auto idx_1 = line.find('=');
        const auto idx_2 = line.find(' ', idx_1);
        // std::cout << idx_1+1 << ' ' <<  idx_2 - idx_1 << '\n';
        // std::cout << '|' << line.substr(idx_1 + 1, idx_2 - idx_1 - 1) << '|' <<'\n';
        p.dim_1 = std::stoi(line.substr(idx_1 + 1, idx_2 - idx_1 - 1));
        // std::cout << __LINE__ << '\n';
        // std::cout << '|' << line.substr(idx_2 + 4, line.size() - idx_2 - 4) << '|' <<'\n';
        p.dim_2 = std::stoi(line.substr(idx_2 + 4, line.size() - idx_2 - 4));
    }
    // If this assumption is incorrect then the code for modifying the screen will need to be updated
    // Might need to use int instead of std::size_t
    assert(p.dim_1 >= 0);
    assert(p.dim_2 >= 0);
    return p;
}

template<typename T, std::size_t d1, std::size_t d2> 
void display_screen(const std::array<std::array<T, d2>, d1>& screen) {
    for (const auto & row : screen) {
        for (const auto & ele : row) {
            std::cout << ele;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

template<std::size_t d1, std::size_t d2> 
std::size_t count_screen(const std::array<std::array<bool, d2>, d1>& screen) {
    std::size_t count = 0;
    for (const auto & row : screen) {
        for (const auto & ele : row) {
            if (ele) count++;
        }
    }
    return count;
}

void apply_instruction(const Parsed& p, std::array<std::array<bool, cols>, rows>& screen) {
    if (p.instr == Instruction::RECT) {
        for (int row_idx = 0; row_idx < p.dim_2; row_idx++) {
            for (int col_idx = 0; col_idx < p.dim_1; col_idx++) {
                screen[row_idx][col_idx] = true;
            }
        }
    } else if (p.instr == Instruction::ROTATE_ROW) {
        const auto temp = screen[p.dim_1];
        for (std::size_t i = 0; i < cols; i++) {
            screen[p.dim_1][i] = false;
        }
        for (std::size_t i = 0; i < cols; i++) {
            screen[p.dim_1][(i+p.dim_2) % cols] = temp[i];
        }
    } else if (p.instr == Instruction::ROTATE_COL) {
        std::array<std::size_t, rows> temp {{}};
        for (std::size_t i = 0; i < rows; i++) {
            temp[i] = screen[i][p.dim_1];
            screen[i][p.dim_1] = false;
        }
        for (std::size_t i = 0; i < rows; i++) {   
            screen[(i + p.dim_2) % rows][p.dim_1] = temp[i];
        }
    }
}


int main(int argc, char* argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  std::array<std::array<bool,cols>, rows> screen{{}};
  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    const auto p = parse_input(line);
    // std::cout << static_cast<std::size_t>(p.instr) << ' ' << p.dim_1 << ' ' << p.dim_2 << '\n';
    apply_instruction(p, screen);
    // display_screen(screen);
  }
  std::cout << count_screen(screen) << '\n';
  return 0;
}
