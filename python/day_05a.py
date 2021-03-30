#!/usr/bin/env python3

def main():
    f = open("../cpp/day_5_data.txt")
    max_seat_id = 0
    for line in f:
        min_row = 0
        max_row = 128
        min_col = 0
        max_col = 8
        for letter in line[0:7]:
            if letter == 'F':
                max_row = (max_row + min_row) / 2
            else:
                min_row = (max_row + min_row) / 2
        for letter in line[7:10]:
            if letter == 'L':
                max_col = (max_col + min_col) / 2
            else:
                min_col = (max_col + min_col) / 2
        max_seat_id = max(max_seat_id, 8 * (max_row - 1) + max_col - 1)
    print(max_seat_id)
    return max_seat_id

if __name__ == "__main__":
    main()
