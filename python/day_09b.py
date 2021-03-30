#!/usr/bin/env python3

import copy

list_size = 25

def main():
    f = open("../cpp/day_9_data.txt")
    values = list()
    all_values = list()
    new_number = 0
    for i in range(0, list_size):
        line = f.readline().strip().replace('\n', '').replace('\r', '')
        values.append(int(line))
    for line in f:
        line = f.readline().strip().replace('\n', '').replace('\r', '')
        new_number = int(line)
        allowed = False
        for i in range(0, list_size):
            if not allowed:
                for j in range(i, list_size):
                    if (values[i] + values[j]) == new_number:
                        allowed = True
                        break
        if allowed:
            values.append(new_number)
            values.pop(0)
            all_values.append(new_number)
        else:
            break
    print("{} does not conform to the pattern".format(new_number))
    start = 0
    end = 0
    list_sum = all_values[start]
    while end < len(all_values) - 1:
        print(list_sum)
        if list_sum == new_number:
            subset = all_values[start:end]
            print(min(subset) + max(subset))
            return all_values[start] + all_values[end]
        elif list_sum > new_number:
            list_sum -= all_values[start]
            start += 1
        elif list_sum < new_number:
            end += 1
            list_sum += all_values[end]
        else:
            print("WTH")
    return 0

if __name__ == "__main__":
    main()
