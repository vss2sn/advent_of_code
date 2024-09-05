#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

enum class InstrType {
    HLF,
    TPL,
    INC,
    JMP,
    JIE,
    JIO,
};

struct Instr {
    Instr (const InstrType type, const int val_1, const int val_2 = 0) : type(type), val_1(val_1), val_2(val_2) {}
    InstrType type;
    int val_1;
    int val_2;
};

int main(int argc, char * argv[]) {
    std::string input = "../input/day_23_input";    
    if (argc > 1) {
        input = argv[1];
    }
    std::ifstream file(input);
    std::string line;
    std::vector<Instr> instructions;
    while(std::getline(file, line)) {
        const auto idx_1 = line.find(' ');
        const std::string_view instr =  std::string_view(line).substr(0, idx_1);
        if (instr == "hlf") {
            instructions.emplace_back(InstrType::HLF, line[idx_1 + 1]-'a');
        } else if (instr == "tpl") {
            instructions.emplace_back(InstrType::TPL, line[idx_1 + 1]-'a');
        } else if (instr == "inc") {
            instructions.emplace_back(InstrType::INC, line[idx_1 + 1]-'a');
        } else if (instr == "jmp") {
            instructions.emplace_back(InstrType::JMP, std::stoi(line.substr(idx_1+1, line.size() - idx_1 - 1)));
        } else if (instr == "jie") {
            const std::size_t delta = (line[idx_1+3] == '+') ? 1 : 0;
            instructions.emplace_back(InstrType::JIE, line[idx_1+1] - 'a', std::stoi(line.substr(idx_1+4+delta, line.size() - idx_1 - 4 - delta)));
        } else if (instr == "jio") {
            const std::size_t delta = (line[idx_1+3] == '+') ? 1 : 0;
            instructions.emplace_back(InstrType::JIO, line[idx_1+1] - 'a', std::stoi(line.substr(idx_1+4+delta, line.size() - idx_1 - 4 - delta)));
        } else {
            std::cout << "This should not happen" << '\n';
            exit(0);
        }
    }
    int ptr = 0;
    std::array<uint64_t, 2> registers {{1,0}};
    while (ptr < instructions.size()) {
        const auto& instr = instructions[ptr];
        if (instr.type == InstrType::HLF) {
            registers[instr.val_1] /= 2;
            ptr++;
        } else if (instr.type == InstrType::TPL) {
            registers[instr.val_1] *= 3;
            ptr++;
        } else if (instr.type == InstrType::INC) {
            registers[instr.val_1]++;
            ptr++;
        } else if (instr.type == InstrType::JMP) {
            ptr += instr.val_1;
        } else if (instr.type == InstrType::JIE) {
            if (registers[instr.val_1] %2 == 0) {
                ptr += instr.val_2;
            } else {
                ptr++;
            }
        } else if (instr.type == InstrType::JIO) {
            if (registers[instr.val_1] == 1) {
                ptr += instr.val_2;
            } else {
                ptr++;
            }
        } else {
            std::cout << "This should not happen" << '\n';
            exit(0);
        }
    }
    std::cout << registers[0] << ' ' << registers[1] << '\n';
    return 0;
}
