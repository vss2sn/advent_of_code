#!/usr/bin/python3

import sys


def getNOps(n, inst_ptr, modes, program):
    ops = list()
    for i in range(0, n):
        rem = modes % 10
        modes = int(modes / 10)
        if rem == 0:
            ops.append(program[program[inst_ptr + i + 1]])
        else:
            ops.append(program[inst_ptr + i + 1])
    return ops


def main():
    # Get input
    file_name = "../input/day_05_input"
    input_position = 1
    if len(sys.argv) > 1 and not sys.argv[1].isnumeric():
        file_name = sys.argv[1]
        input_position += 1
    file = open(file_name)
    program = [int(ele) for ele in file.readline().strip("\n").split(",")]

    # Solve
    inst_ptr = 0
    output = -1
    while program[inst_ptr] != 99:
        instr = program[inst_ptr]
        opcode = instr % 100
        modes = int(instr / 100)

        if opcode == 1:
            ops = getNOps(2, inst_ptr, modes, program)
            program[program[inst_ptr + 3]] = ops[0] + ops[1]
            inst_ptr += 4
        elif opcode == 2:
            ops = getNOps(2, inst_ptr, modes, program)
            program[program[inst_ptr + 3]] = ops[0] * ops[1]
            inst_ptr += 4
        elif opcode == 3:
            program[program[inst_ptr + 1]] = int(
                sys.argv[input_position]
            )  # Based on input
            # print("Input: {}".format(int(sys.argv[input_position])))
            inst_ptr += 2
            input_position += 1
        elif opcode == 4:
            # print(program[program[inst_ptr + 1]])
            output = program[program[inst_ptr + 1]]
            inst_ptr += 2
        elif opcode == 5:
            ops = getNOps(2, inst_ptr, modes, program)
            if ops[0] != 0:
                inst_ptr = ops[1]
            else:
                inst_ptr += 3
        elif opcode == 6:
            ops = getNOps(2, inst_ptr, modes, program)
            if ops[0] == 0:
                inst_ptr = ops[1]
            else:
                inst_ptr += 3
        elif opcode == 7:
            ops = getNOps(2, inst_ptr, modes, program)
            if ops[0] < ops[1]:
                program[program[inst_ptr + 3]] = 1
            else:
                program[program[inst_ptr + 3]] = 0
            inst_ptr += 4
        elif opcode == 8:
            ops = getNOps(2, inst_ptr, modes, program)
            if ops[0] == ops[1]:
                program[program[inst_ptr + 3]] = 1
            else:
                program[program[inst_ptr + 3]] = 0
            inst_ptr += 4
    print(output)
    return output


if __name__ == "__main__":
    main()
