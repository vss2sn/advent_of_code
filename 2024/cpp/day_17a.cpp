#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector> 
#include <cassert> 


struct Computer {
    std::array<int, 3> registers = {{0,0,0}};

    int get_value_of_combo_operand(const int operand) {
        if (operand >= 0 && operand <= 3) return operand;
        if (operand == 4) return registers[0];
        if (operand == 5) return registers[1];
        if (operand == 6) return registers[2];
        std::cout << "Invalid program" << '\n';
        exit(0);
        return -1;
    }

    std::vector<int> run(const std::vector<int>& instructions) {
        int instr_ptr = 0;
        std::vector<int> output;
        while(instr_ptr < instructions.size()) {
            if (instructions[instr_ptr] == 0) {
                adv(instructions[instr_ptr+1]);
                instr_ptr+=2;
            } else if (instructions[instr_ptr] == 1) {
                bxl(instructions[instr_ptr+1]);
                instr_ptr+=2;
            } else if (instructions[instr_ptr] == 2) {
                bst(instructions[instr_ptr+1]);
                instr_ptr+=2;
            } else if (instructions[instr_ptr] == 3) {
                jnz(instructions[instr_ptr+1], instr_ptr);
            } else if (instructions[instr_ptr] == 4) {
                bxc(instructions[instr_ptr+1]);
                instr_ptr+=2;
            } else if (instructions[instr_ptr] == 5) {
                out(instructions[instr_ptr+1], output);
                instr_ptr+=2;
            } else if (instructions[instr_ptr] == 6) {
                bdv(instructions[instr_ptr+1]);
                instr_ptr+=2;
            } else if (instructions[instr_ptr] == 7) {
                cdv(instructions[instr_ptr+1]);
                instr_ptr+=2;
            }
        }
        return output;
    }

    void adv(const int operand) {
        registers[0] = registers[0] / static_cast<int>(std::pow(2, get_value_of_combo_operand(operand)));
    }

    void bxl(const int operand) {
        registers[1] = registers[1] ^ operand;
    }

    void bst(const int operand) {
        registers[1] = get_value_of_combo_operand(operand) % 8;
    }

    void jnz(const int operand, int& instr_ptr) {
        if (registers[0] != 0) instr_ptr = operand;
        else instr_ptr += 2;
    }

    void bxc(const int operand) {
        registers[1] = registers[1] ^ registers[2];
    }

    void out(const int operand, std::vector<int>& output) {
        const auto result = std::to_string(get_value_of_combo_operand(operand) % 8);
        for (const auto ele : result) {
            output.push_back(ele - '0');
        }
    }
    
    void bdv(const int operand) {
        registers[1] = registers[0] / static_cast<int>(std::pow(2, get_value_of_combo_operand(operand)));
    }

    void cdv(const int operand) {
        registers[2] = registers[0] / static_cast<int>(std::pow(2, get_value_of_combo_operand(operand)));
    } 
};

int main(int argc, char* argv[]) {
    std::string input = "../input/day_17_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    Computer computer;

    // Tests 
    // computer.registers = {{0,0,9}};
    // computer.run({2,6});
    // std::cout << '\n';
    // assert(computer.registers[1] == 1);
    // std::cout << "--------\n";

    // computer.registers = {{10,0,0}};
    // computer.run({5,0,5,1,5,4 });
    // std::cout << '\n';
    // // assert(computer.registers[1] == 1);
    // std::cout << "--------\n";

    // computer.registers = {{2024,0,0}};
    // computer.run({0,1,5,4,3,0});
    // std::cout << '\n';
    // assert(computer.registers[0] == 0);
    // std::cout << "--------\n";

    // computer.registers = {{0,29,0}};
    // computer.run({1,7});
    // std::cout << '\n';
    // assert(computer.registers[1] == 26);
    // std::cout << "--------\n";

    // computer.registers = {{0,2024, 43690}};
    // computer.run({4,0});
    // std::cout << '\n';
    // assert(computer.registers[1] == 44354);
    // exit(0);
    // std::cout << "--------\n";

    std::vector<int> instructions;
    std::getline(file, line);
    computer.registers[0] = std::stoi(line.substr(12, line.size() - 12));
    std::getline(file, line);
    computer.registers[1] = std::stoi(line.substr(12, line.size() - 12));
    std::getline(file, line);
    computer.registers[2] = std::stoi(line.substr(12, line.size() - 12));
    std::getline(file, line);
    std::getline(file, line);
    std::size_t start = 9;
    std::size_t end = line.find(',', start);
    while(end != std::string::npos) {
        instructions.push_back(std::stoi(line.substr(start, end - start)));
        start = end + 1;
        end = line.find(',', start);
    }
    instructions.push_back(std::stoi(line.substr(start, line.size() - start)));
    const auto output = computer.run(instructions);
    for (int i = 0; i < output.size() - 1; i++) {
        std::cout << output[i] << ','; 
    }
    std::cout << output.back() << '\n';
    return 0;
}