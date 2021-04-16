#!/usr/bin/python3

import sys


def meetsConditions(num):
    prev_val = 10
    n_repeat = 0
    repeat = False
    while num > 0:
        val = num % 10
        num = int(num / 10)
        if val > prev_val:
            return False
        if prev_val == val:
            n_repeat = n_repeat + 1
        else:
            if n_repeat == 1:
                repeat = True
            n_repeat = 0
        prev_val = val
    if n_repeat == 1:
        repeat = True
    return repeat


if __name__ == "__main__":
    # Get input
    file_name = "../input/day_04_input"
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    file = open(file_name)
    val_range = [int(ele) for ele in file.readline().strip("\n").split("-")]

    # Solve
    count = 0
    for num in range(val_range[0], val_range[1] + 1):
        if meetsConditions(num):
            count += 1
    print(count)
