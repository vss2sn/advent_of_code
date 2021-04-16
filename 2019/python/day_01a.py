#!/usr/bin/python3

import sys


def main():
    # Get input
    file_name = "../input/day_01_input"
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    file = open(file_name)
    input = [int(ele) for ele in file]

    # Solve
    total_fuel = 0
    for mass in input:
        total_fuel = total_fuel + int(mass / 3) - 2
    print(total_fuel)
    return total_fuel


if __name__ == "__main__":
    main()
