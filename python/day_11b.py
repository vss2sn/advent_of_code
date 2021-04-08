#!/usr/bin/env python3

directions = [[1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0], [-1, -1], [0, -1], [1, -1]]


def getOccupiedNeighbours(i, j, seats):
    count = 0
    for dir in directions:
        new_point = [i, j]
        while True:
            new_point = [sum(x) for x in zip(new_point, dir)]
            if (
                new_point[0] < 0
                or new_point[0] >= len(seats)
                or new_point[1] < 0
                or new_point[1] >= len(seats[i])
            ):
                break
            elif seats[new_point[0]][new_point[1]] == "#":
                count += 1
                break
            elif seats[new_point[0]][new_point[1]] == "L":
                break
    return count


def UpdateSeating(seats, nos):
    for i in range(0, len(seats)):
        for j in range(0, len(seats[i])):
            nos[i][j] = getOccupiedNeighbours(i, j, seats)
    changed = False
    for i in range(0, len(seats)):
        for j in range(0, len(seats[i])):
            if seats[i][j] == "L" and nos[i][j] == 0:
                changed = True
                seats[i][j] = "#"
            elif seats[i][j] == "#" and nos[i][j] >= 5:
                changed = True
                seats[i][j] = "L"
    return changed


def CountOccupied(seats):
    count = 0
    for row in seats:
        for col in row:
            if col == "#":
                count += 1
    return count


def printSeats(seats):
    for line in seats:
        print(line)


def main():
    f = open("../input/day_11_input")
    seats = list()
    for line in f:
        line = line.strip().replace("\r", "").replace("\n", "")
        seats.append([letter for letter in line])
    nos = []
    for i in range(0, len(seats)):
        nos.append([0] * len(seats[i]))

    changed = True
    while changed:
        changed = UpdateSeating(seats, nos)
    count = CountOccupied(seats)
    print(count)
    return count


if __name__ == "__main__":
    main()
