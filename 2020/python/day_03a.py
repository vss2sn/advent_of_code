#!/usr/bin/env python3
def countTreesInPath(forest, step_x, step_y):
    tree = "#"
    n_rows = len(forest)
    x = 0
    y = 0
    count = 0
    n_cols = len(forest[0])
    while y < n_rows:
        if forest[y][x] == "#":
            count += 1
        x += step_x
        x %= n_cols
        y += step_y
    return count


def main():
    f = open("../input/day_03_input")
    forest = [line.strip() for line in f.readlines()]
    print(countTreesInPath(forest, 3, 1))


if __name__ == "__main__":
    main()
