#!/usr/bin/python3

import sys


def recursiveOrbitCount(current_object, is_orbitted_by, n_orbits):
    if current_object not in is_orbitted_by.keys():
        n_orbits[current_object] = 0
    else:
        n_orbits[current_object] = 0
        for orbitting_object in is_orbitted_by[current_object]:
            recursiveOrbitCount(orbitting_object, is_orbitted_by, n_orbits)
            n_orbits[current_object] += n_orbits[orbitting_object] + 1


def main():
    # Get input
    file_name = "../input/day_06_input"
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    file = open(file_name)
    orbits = dict()
    is_orbitted_by = dict()
    n_orbits = dict()
    input = list()
    for line in file:
        line = line.strip("\n")
        input.append(line)

    # Solve
    for line in input:
        orbit = line.split(")")
        orbits[orbit[1]] = orbit[0]
        if orbit[0] not in is_orbitted_by.keys():
            is_orbitted_by[orbit[0]] = set()
        is_orbitted_by[orbit[0]].add(orbit[1])
    recursiveOrbitCount("COM", is_orbitted_by, n_orbits)
    total = 0
    for n in n_orbits.values():
        total += n
    print(total)
    return total


if __name__ == "__main__":
    main()
