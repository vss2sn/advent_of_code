#!/usr/bin/python3

import math
import sys


def GetWireCoordinatesList(input):
    init_x = 0
    init_y = 0
    wire = list()
    for inst in input:
        if inst[0] == "R":
            for inc in range(1, int(inst[1:]) + 1):
                init_x += 1
                wire.append((init_x, init_y))
        elif inst[0] == "U":
            for inc in range(1, int(inst[1:]) + 1):
                init_y += 1
                wire.append((init_x, init_y))
        elif inst[0] == "L":
            for inc in range(1, int(inst[1:]) + 1):
                init_x -= 1
                wire.append((init_x, init_y))
        elif inst[0] == "D":
            for inc in range(1, int(inst[1:]) + 1):
                init_y -= 1
                wire.append((init_x, init_y))
    return wire


def GetWireCoordinatesSet(input):
    init_x = 0
    init_y = 0
    wire = set()
    for inst in input:
        if inst[0] == "R":
            for inc in range(1, int(inst[1:]) + 1):
                init_x += 1
                wire.add((init_x, init_y))
        elif inst[0] == "U":
            for inc in range(1, int(inst[1:]) + 1):
                init_y += 1
                wire.add((init_x, init_y))
        elif inst[0] == "L":
            for inc in range(1, int(inst[1:]) + 1):
                init_x -= 1
                wire.add((init_x, init_y))
        elif inst[0] == "D":
            for inc in range(1, int(inst[1:]) + 1):
                init_y -= 1
                wire.add((init_x, init_y))
    return wire


def main():
    # Get input
    file_name = "../input/day_03_input"
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    file = open(file_name)
    input1 = file.readline().strip("\n").split(",")
    input2 = file.readline().strip("\n").split(",")

    # Solve
    listwire = GetWireCoordinatesList(input1)
    setwire = GetWireCoordinatesSet(input2)
    man_dist = math.inf
    for coor in listwire:
        if coor in setwire:
            man_dist = min(man_dist, abs(coor[0]) + abs(coor[1]))
    print(man_dist)
    return man_dist


if __name__ == "__main__":
    main()
