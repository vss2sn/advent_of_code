#!/usr/bin/env python3

def getOccupiedNeighbours(i, j, seats):
    count = 0
    for ii in range(i - 1, i + 2):
        if ii < 0 or ii >= len(seats):
            continue
        for jj in range(j - 1, j + 2):
            if jj < 0 or jj >= len(seats[ii]):
                continue
            if ii == i and jj == j:
                continue
            if seats[ii][jj] == '#':
                count += 1
    return count

def UpdateSeating(seats, nos):
    for i in range(0, len(seats)):
        for j in range(0, len(seats[i])):
            nos[i][j] = getOccupiedNeighbours(i, j, seats)
    changed = False
    for i in range(0, len(seats)):
        for j in range(0, len(seats[i])):
            if seats[i][j] == 'L' and nos[i][j] == 0:
                changed = True
                seats[i][j] = '#'
            elif seats[i][j] == '#' and nos[i][j] >= 4:
                changed = True
                seats[i][j] = 'L'
    return changed

def CountOccupied(seats):
    count = 0
    for row in seats:
        for col in row:
            if col == '#':
                count += 1
    return count

def printSeats(seats):
    for line in seats:
        print(line)

def main():
    f = open("../cpp/day_11_data.txt")
    seats = list()
    for line in f:
        line = line.strip().replace('\r', '').replace('\n', '')
        seats.append([letter for letter in line])
    nos = []
    for i in range(0, len(seats)):
        nos.append([0] * len(seats[i]))

    changed = True
    while changed:
        changed = UpdateSeating(seats, nos)
    count =  CountOccupied(seats)
    print(count)
    return count

if __name__ == "__main__":
    main();
