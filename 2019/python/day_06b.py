#!/usr/bin/python3

import sys


def travel(steps, cur, dest, is_orbitted_by, orbits, reached_before):
    if cur == dest:
        return True, steps
    if cur in reached_before:
        return False, -1
    reached_before.add(cur)
    if cur in is_orbitted_by.keys():
        for orbitting_object in is_orbitted_by[cur]:
            reached, final_step_count = travel(
                steps + 1,
                orbitting_object,
                dest,
                is_orbitted_by,
                orbits,
                reached_before,
            )
            if reached:
                return reached, final_step_count
    if cur in orbits.keys():
        reached, final_step_count = travel(
            steps + 1, orbits[cur], dest, is_orbitted_by, orbits, reached_before
        )
        if reached:
            return reached, final_step_count
    return False, -1


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
    reached_before = set()
    reached, n_steps = travel(0, "YOU", "SAN", is_orbitted_by, orbits, reached_before)
    print(n_steps - 2)
    return n_steps - 2


if __name__ == "__main__":
    main()
