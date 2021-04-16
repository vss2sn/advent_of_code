#!/usr/bin/python3

import copy
import sys


def main():
    # Get program
    file_name = "../input/day_02_input"
    desired_output = 19690720
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    file = open(file_name)
    noun_verb_list = list(range(0, 100))
    program_str = file.readline()

    # Solve
    ans = -1
    for noun in noun_verb_list:
        for verb in noun_verb_list:
            if noun == verb:
                continue
            program = list(map(int, program_str.split(",")))
            program[1] = noun
            program[2] = verb
            inst_ptr = 0
            while program[inst_ptr] != 99:
                if program[inst_ptr] == 1:
                    program[program[inst_ptr + 3]] = (
                        program[program[inst_ptr + 1]] + program[program[inst_ptr + 2]]
                    )
                elif program[inst_ptr] == 2:
                    program[program[inst_ptr + 3]] = (
                        program[program[inst_ptr + 1]] * program[program[inst_ptr + 2]]
                    )
                inst_ptr = inst_ptr + 4
            if program[0] == desired_output:
                ans = noun * 100 + verb
                print(ans)
                return ans
    return ans


if __name__ == "__main__":
    main()
