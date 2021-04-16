#!/usr/bin/env python3

import re


def main():
    f = open("../input/day_02_input")
    match_pattern = re.compile(r"(\d+)-(\d+) ([a-z]): (.*)\n")
    count = 0
    for line in f:
        low, high, letter, pwd = re.match(match_pattern, line).groups()
        n = pwd.count(letter)
        if n >= int(low) and n <= int(high):
            count += 1
    print(count)
    return count


if __name__ == "__main__":
    main()
