#!/usr/bin/python3

import sys
import itertools


class IntcodeComputer:
    def __init__(self, program_str):
        self.program_str = program_str
        self.program = list(map(int, program_str.split(",")))

    def reset(self):
        self.program = list(map(int, self.program_str.split(",")))

    def getNOps(self, n, inst_ptr, modes):
        ops = list()
        for i in range(0, n):
            rem = modes % 10
            modes = int(modes / 10)
            if rem == 0:
                ops.append(self.program[self.program[inst_ptr + i + 1]])
            else:
                ops.append(self.program[inst_ptr + i + 1])
        return ops

    def compute(self, input):
        inst_ptr = 0
        output = 0
        while self.program[inst_ptr] != 99:
            # print(inst_ptr)
            instr = self.program[inst_ptr]
            opcode = instr % 100
            modes = int(instr / 100)

            if opcode == 1:
                ops = self.getNOps(2, inst_ptr, modes)
                self.program[self.program[inst_ptr + 3]] = ops[0] + ops[1]
                inst_ptr += 4
            elif opcode == 2:
                ops = self.getNOps(2, inst_ptr, modes)
                self.program[self.program[inst_ptr + 3]] = ops[0] * ops[1]
                inst_ptr += 4
            elif opcode == 3:
                self.program[self.program[inst_ptr + 1]] = input.pop(0)
                inst_ptr += 2
            elif opcode == 4:
                output = self.program[self.program[inst_ptr + 1]]
                inst_ptr += 2
                return output
            elif opcode == 5:
                ops = self.getNOps(2, inst_ptr, modes)
                if ops[0] != 0:
                    inst_ptr = ops[1]
                else:
                    inst_ptr += 3
            elif opcode == 6:
                ops = self.getNOps(2, inst_ptr, modes)
                if ops[0] == 0:
                    inst_ptr = ops[1]
                else:
                    inst_ptr += 3
            elif opcode == 7:
                ops = self.getNOps(2, inst_ptr, modes)
                if ops[0] < ops[1]:
                    self.program[self.program[inst_ptr + 3]] = 1
                else:
                    self.program[self.program[inst_ptr + 3]] = 0
                inst_ptr += 4
            elif opcode == 8:
                ops = self.getNOps(2, inst_ptr, modes)
                if ops[0] == ops[1]:
                    self.program[self.program[inst_ptr + 3]] = 1
                else:
                    self.program[self.program[inst_ptr + 3]] = 0
                inst_ptr += 4
        return output


def TestAmplifierConfigurationAndReset(computer, phase_setting):
    output = 0
    for i in range(0, len(phase_setting)):
        output = computer.compute([phase_setting[i], output])
        computer.reset()
    return output


def main():
    # Get program and input
    file_name = "../input/day_07_input"
    if len(sys.argv) > 1 and not sys.argv[1].isnumeric():
        file_name = sys.argv[1]
    file = open(file_name)
    program_str = file.readline().strip("\n")

    # Solve
    computer = IntcodeComputer(program_str)

    max_output = 0
    for phase_setting in itertools.permutations([0, 1, 2, 3, 4]):
        output = TestAmplifierConfigurationAndReset(computer, phase_setting)
        if output > max_output:
            max_output = output
    print(max_output)
    return max_output


if __name__ == "__main__":
    main()
