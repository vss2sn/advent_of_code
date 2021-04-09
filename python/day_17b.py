#!/usr/bin/python3
import itertools
import operator
import copy


def parseInputAndCreateCells(file_name, n_dims):
    f = open(file_name)
    raw_data = f.readlines()
    cells = set()
    for y, line in enumerate(raw_data):
        # line = line.strip().replace('\r', '').replace('\n', '')
        for x, cell_val in enumerate(line):
            if cell_val == "#":
                cells.add((x, y) + (0,) * (n_dims - 2))
    return cells


def getBounds(cells, n_dims):
    res = list()
    for i in range(n_dims):
        res.append([0] * 2)
    for i in range(n_dims):
        res[i][0] = min(cells, key=lambda x: x[i])[i] - 1
        res[i][1] = max(cells, key=lambda x: x[i])[i] + 2
    return res


def countActiveNeighbours(cell, cells, n_dims):
    bounds = getBounds(cells, n_dims)
    diffs = itertools.product([-1, 0, 1], repeat=n_dims)
    n_count = 0
    # TODO(vss): List comprehension?
    for diff in diffs:
        if diff != (0,) * n_dims:
            if tuple(map(operator.add, cell, diff)) in cells:
                n_count += 1
    return n_count


def stepHelper(cell, cells, updated_cells, n_dims, bounds):
    if len(cell) == n_dims:
        n_cells = countActiveNeighbours(cell, cells, n_dims)
        if (n_cells == 2 or n_cells == 3) and cell in cells:
            updated_cells.add(cell)
        elif cell not in cells and n_cells == 3:
            updated_cells.add(cell)
    else:
        dim = len(cell)
        for val in range(bounds[dim][0], bounds[dim][1]):
            new_cell = cell + (val,)
            stepHelper(new_cell, cells, updated_cells, n_dims, bounds)


def step(cells, n_dims, bounds):
    updated_cells = set()
    cell = tuple()
    stepHelper(cell, cells, updated_cells, n_dims, bounds)
    return updated_cells


def main():
    n_steps = 6
    n_dims = 4
    file_name = "../input/day_17_input"

    cells = parseInputAndCreateCells(file_name, n_dims)
    for _ in range(n_steps):
        bounds = getBounds(cells, n_dims)
        cells = step(cells, n_dims, bounds)
    print(len(cells))


if __name__ == "__main__":
    main()
