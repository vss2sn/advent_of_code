#!/usr/bin/env python3

def main():
    total = 2020
    f = open("../cpp/day_1_data.txt")
    numbers = set()
    for line in f:
        line = line.strip().replace('\n', '').replace('\r', '')
        num = int(line)
        diff = total - num
        if diff in numbers:
            print(diff * num)
            return diff * num
        else:
            numbers.add(num)
    return 0

if __name__ == "__main__":
    main()        
