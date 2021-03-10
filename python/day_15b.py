#!/usr/bin/env python3
import copy
def main():
    f = open("../cpp/day_15_data.txt")
    inputs = f.readline().strip().replace('\r', '').replace('\n', '').split(',')
    numbers = dict()
    count = 0
    for input in inputs:
        numbers[int(input)] = count
        count += 1
    cur = 0;
    while count < 29999999:
        if cur in numbers.keys():
            temp = copy.deepcopy(cur)
            cur = count - numbers[cur]
            numbers[temp] = count
        else:
            numbers[cur] = count
            cur = 0
        count += 1
    print(cur)
    return cur

if __name__ == "__main__":
    main()
