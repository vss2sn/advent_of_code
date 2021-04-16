#!/usr/bin/python3

import sys


def getRecursiveFuel(mass):
    divisor = 3
    subtract = 2
    minimum_mass = divisor * subtract
    if mass < minimum_mass:
        return 0
    fuel = int(mass / divisor) - subtract
    return fuel + getRecursiveFuel(fuel)


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
        total_fuel = total_fuel + getRecursiveFuel(mass)
    print(total_fuel)
    return total_fuel


if __name__ == "__main__":
    main()
