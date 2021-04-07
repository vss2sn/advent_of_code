#!/usr/bin/env python3

def main():
    f = open("../input/day_12_input")
    position = [0, 0]
    dir = 0
    for line in f:
        line = line.strip().replace('\r', '').replace('\n', '')
        if line[0] == 'N':
            position[1] += int(line[1:])
        elif line[0] == 'E':
            position[0] += int(line[1:])
        elif line[0] == 'S':
            position[1] -= int(line[1:])
        elif line[0] == 'W':
            position[0] -= int(line[1:])
        elif line[0] == 'F':
            if dir == 0:
                position[0] += int(line[1:])
            elif dir == 1:
                position[1] += int(line[1:])
            elif dir == 2:
                position[0] -= int(line[1:])
            elif dir == 3:
                position[1] -= int(line[1:])
        elif line[0] == 'R':
            dir -= 1 * (int(line[1:])/90)
            dir %= 4
        elif line[0] == 'L':
            dir += 1 * (int(line[1:])/90)
            while dir < 0:
                dir += 4
            dir %= 4
        else:
            print("WTH")
    print(abs(position[0]) + abs(position[1]))
    return abs(position[0]) + abs(position[1])

if __name__ == "__main__":
    main()
