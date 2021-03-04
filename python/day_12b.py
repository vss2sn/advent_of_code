#!/usr/bin/env python3

def main():
    f = open("../cpp/day_12_data.txt")
    wp_pos = [10, 1]
    ferry_pos = [0, 0]
    for line in f:
        line = line.strip().replace('\r', '').replace('\n', '')
        if line[0] == 'N':
            wp_pos[1] += int(line[1:])
        elif line[0] == 'E':
            wp_pos[0] += int(line[1:])
        elif line[0] == 'S':
            wp_pos[1] -= int(line[1:])
        elif line[0] == 'W':
            wp_pos[0] -= int(line[1:])
        elif line[0] == 'F':
            ferry_pos[0] += wp_pos[0] * int(line[1:])
            ferry_pos[1] += wp_pos[1] * int(line[1:])
        elif line[0] == 'R':
            for i in range(0, int(int(line[1:])/90)):
                temp = wp_pos[0]
                wp_pos[0] = wp_pos[1]
                wp_pos[1] = -temp;
        elif line[0] == 'L':
            for i in range(0, int(int(line[1:])/90)):
                temp = wp_pos[0]
                wp_pos[0] = -wp_pos[1]
                wp_pos[1] = temp;
        else:
            print("WTH")
    print(abs(ferry_pos[0]) + abs(ferry_pos[1]))
    return abs(ferry_pos[0]) + abs(ferry_pos[1])

if __name__ == "__main__":
    main()
