#!/usr/bin/env python3

import re


def main():
    f = open("../input/day_02_input")
    pattern = re.compile(r"(\d+)-(\d+) ([a-z]): (.*)\n")
    count = 0
    for line in f:
        low, high, letter, pwd = re.match(pattern, line).groups()
        if (pwd[int(low) - 1] == letter) ^ (pwd[int(high) - 1] == letter):
            count += 1
    print(count)
    return count


if __name__ == "__main__":
    main()
