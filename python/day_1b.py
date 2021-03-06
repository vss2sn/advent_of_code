#!/usr/bin/env python3

def main():
    total = 2020
    f = open("../cpp/day_1_data.txt")
    numbers = list()
    for line in f:
        line = line.strip().replace('\n', '').replace('\r', '')
        numbers.append(int(line))
        numbers.sort()
    for start in range(0, len(numbers)):
        mid = start + 1
        end = len(numbers) - 1
        while (mid < end):
            rem = total - numbers[start] - numbers[mid] - numbers[end]
            if rem == 0:
                print(numbers[start] * numbers[mid] * numbers[end])
                return numbers[start] * numbers[mid] * numbers[end]
            elif rem > 0:
                mid += 1
            elif rem < 0:
                end -= 1
            else:
                print("WTH")
    return 0

if __name__ == "__main__":
    main()
