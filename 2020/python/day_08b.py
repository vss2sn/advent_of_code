#!/usr/bin/env python3

import copy


def detectLoop(acc, line, lines):
    n_lines = len(lines)
    execed_list = [False] * n_lines
    loop_detected = False
    while line < n_lines:
        if execed_list[line]:
            loop_detected = True
            break
        execed_list[line] = True
        if lines[line][0:3] == "nop":
            line = line + 1
        elif lines[line][0:3] == "jmp":
            line = line + int(lines[line][3:])
        elif lines[line][0:3] == "acc":
            acc = acc + int(lines[line][3:])
            line = line + 1
        else:
            print("WTH")
    return loop_detected, acc


def main():
    f = open("../input/day_08_input")
    lines = [line.strip().replace("\n", "").replace("\r", "") for line in f]
    n_lines = len(lines)
    execed_list = [False] * n_lines
    line = 0
    acc = 0
    while line < n_lines:
        execed_list[line] = True
        if lines[line][0:3] == "nop":
            prev_line = copy.deepcopy(line)
            line = line + int(lines[line][3:])
            loop_detected, acc_val = detectLoop(
                copy.deepcopy(acc), copy.deepcopy(line), lines
            )
            if not loop_detected:
                print(acc_val)
                return acc_val
            else:
                line = prev_line + 1
        elif lines[line][0:3] == "jmp":
            prev_line = copy.deepcopy(line)
            line = line + 1
            loop_detected, acc_val = detectLoop(
                copy.deepcopy(acc), copy.deepcopy(line), lines
            )
            if not loop_detected:
                print(acc_val)
                return acc_val
            else:
                line = prev_line + int(lines[prev_line][3:])
        elif lines[line][0:3] == "acc":
            acc = acc + int(lines[line][3:])
            line = line + 1
        else:
            print("WTH")
    print(acc_val)
    return acc


if __name__ == "__main__":
    main()
