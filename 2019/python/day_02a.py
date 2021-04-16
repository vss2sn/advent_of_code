#!/usr/bin/python3

import sys


def main():
    # Get input
    file_name = "../input/day_02_input"
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    file = open(file_name)
    program = list(map(int, file.readline().split(",")))

    # Modify program according to puzzle instructions
    program[1] = 12
    program[2] = 2

    # Solve
    index = 0
    while program[index] != 99:
        if program[index] == 1:
            program[program[index + 3]] = (
                program[program[index + 1]] + program[program[index + 2]]
            )
        elif program[index] == 2:
            program[program[index + 3]] = (
                program[program[index + 1]] * program[program[index + 2]]
            )
        index = index + 4
    print(program[0])


if __name__ == "__main__":
    main()
