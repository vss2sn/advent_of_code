#!/usr/bin/env python3

import re


def main():
    f = open("../input/day_14_input")
    mem_pattern = re.compile(r"mem\[(\d+)\] = (\d+)(.*?)")
    mem = dict()
    for line in f:
        line = line.strip().replace("\n", "").replace("\r", "")
        if line[0:4] == "mask":
            mask = line.split()[-1]
            clear_mask = [1 if char == "X" else 0 for char in mask]
            set_mask = [1 if char == "1" else 0 for char in mask]
        else:
            address, value, __ = mem_pattern.match(line).groups()
            value_list = format(int(value), "#038b")
            num = list()
            for i in range(0, 36):
                num.append((int(value_list[i + 2]) & clear_mask[i]) | set_mask[i])
            mem[address] = num
    total = 0
    for ele in mem.values():
        res = 0
        for e in ele:
            res = (res << 1) | e
        total += res
    print(total)
    return total


if __name__ == "__main__":
    main()
