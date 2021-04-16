#!/usr/bin/env python3


def main():
    f = open("../input/day_08_input")
    lines = [line.strip().replace("\n", "").replace("\r", "") for line in f]
    n_lines = len(lines)
    execed_list = [False] * n_lines
    line = 0
    acc = 0
    while line < n_lines:
        if execed_list[line]:
            print(acc)
            return acc
        else:
            execed_list[line] = True
        if lines[line][0:3] == "nop":
            line = line + 1
        elif lines[line][0:3] == "jmp":
            line = line + int(lines[line][3:])
        elif lines[line][0:3] == "acc":
            acc += int(lines[line][3:])
            line = line + 1
        else:
            print("WTH")
    return 0


if __name__ == "__main__":
    main()
