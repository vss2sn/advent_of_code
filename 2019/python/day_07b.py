#!/usr/bin/python3

import sys
import itertools


class AmplifierConfigurationProgram:
    def __init__(self, program_str):
        self.program_str = program_str
        self.program = list(map(int, program_str.split(",")))
        self.complete = False
        self.inst_ptr = 0

    def reset(self):
        self.program = list(map(int, program_str.split(",")))
        self.complete = False
        self.inst_ptr = 0

    def getNOps(self, n, modes):
        ops = list()
        for i in range(0, n):
            rem = modes % 10
            modes = int(modes / 10)
            if rem == 0:
                ops.append(self.program[self.program[self.inst_ptr + i + 1]])
            else:
                ops.append(self.program[self.inst_ptr + i + 1])
        return ops

    def compute(self, input):
        output = None
        while self.program[self.inst_ptr] != 99:
            # print(inst_ptr)
            instr = self.program[self.inst_ptr]
            opcode = instr % 100
            modes = int(instr / 100)

            if opcode == 1:
                ops = self.getNOps(2, modes)
                self.program[self.program[self.inst_ptr + 3]] = ops[0] + ops[1]
                self.inst_ptr += 4
            elif opcode == 2:
                ops = self.getNOps(2, modes)
                self.program[self.program[self.inst_ptr + 3]] = ops[0] * ops[1]
                self.inst_ptr += 4
            elif opcode == 3:
                self.program[self.program[self.inst_ptr + 1]] = input.pop(0)
                self.inst_ptr += 2
            elif opcode == 4:
                output = self.program[self.program[self.inst_ptr + 1]]
                self.inst_ptr += 2
                return output
            elif opcode == 5:
                ops = self.getNOps(2, modes)
                if ops[0] != 0:
                    self.inst_ptr = ops[1]
                else:
                    self.inst_ptr += 3
            elif opcode == 6:
                ops = self.getNOps(2, modes)
                if ops[0] == 0:
                    self.inst_ptr = ops[1]
                else:
                    self.inst_ptr += 3
            elif opcode == 7:
                ops = self.getNOps(2, modes)
                if ops[0] < ops[1]:
                    self.program[self.program[self.inst_ptr + 3]] = 1
                else:
                    self.program[self.program[self.inst_ptr + 3]] = 0
                self.inst_ptr += 4
            elif opcode == 8:
                ops = self.getNOps(2, modes)
                if ops[0] == ops[1]:
                    self.program[self.program[self.inst_ptr + 3]] = 1
                else:
                    self.program[self.program[self.inst_ptr + 3]] = 0
                self.inst_ptr += 4
        self.complete = True
        return output


class IntcodeComputer:
    def __init__(self):
        self.AmplifierConfigurationPrograms = list()

    def addAmplifierConfigurationProgram(self, program_str):
        self.AmplifierConfigurationPrograms.append(
            AmplifierConfigurationProgram(program_str)
        )

    def runAmplifierConfigurationProgram(self, index, input):
        return self.AmplifierConfigurationPrograms[index].compute(input)

    def resetAmplifierConfigurationProgram(self, ndex):
        self.AmplifierConfigurationPrograms.reset()

    def reset(self):
        self.AmplifierConfigurationPrograms.clear()


def TestAmplifierConfigurationAndReset(computer, program_str, phase_setting):
    output = 0
    for i in range(0, len(phase_setting)):
        computer.addAmplifierConfigurationProgram(program_str)
        output = computer.runAmplifierConfigurationProgram(
            i, [phase_setting[i], output]
        )
    computer.reset()
    return output


def TestAmplifierConfigurationWithFeedbackAndReset(
    computer, program_str, phase_setting
):
    output = 0
    last_valid = None
    for i in range(0, len(phase_setting)):
        computer.addAmplifierConfigurationProgram(program_str)
    for i in range(0, len(phase_setting)):
        output = computer.runAmplifierConfigurationProgram(
            i, [phase_setting[i], output]
        )
    while not computer.AmplifierConfigurationPrograms[-1].complete:
        for i in range(0, len(phase_setting)):
            output = computer.runAmplifierConfigurationProgram(i, [output])
            if output is not None:
                last_valid = output
    computer.reset()
    return last_valid


def main():
    # Get program and input
    file_name = "../input/day_07_input"
    if len(sys.argv) > 1 and not sys.argv[1].isnumeric():
        file_name = sys.argv[1]
    file = open(file_name)
    program_str = file.readline().strip("\n")

    # Solve
    computer = IntcodeComputer()
    max_output = 0
    for phase_setting in itertools.permutations([5, 6, 7, 8, 9]):
        output = TestAmplifierConfigurationWithFeedbackAndReset(
            computer, program_str, phase_setting
        )
        if output > max_output:
            max_output = output
    print(max_output)
    return max_output


if __name__ == "__main__":
    main()
