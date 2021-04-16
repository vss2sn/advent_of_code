#!/usr/bin/env python3

import copy


def main():
    list_size = 25
    f = open("../input/day_09_input")
    values = list()
    all_values = list()
    new_number = 0
    for i in range(0, list_size):
        line = f.readline().strip().replace("\n", "").replace("\r", "")
        values.append(int(line))
    for line in f:
        new_number = int(line)
        allowed = False
        # TODO(vss): Optimize
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
    start = 0
    end = 0
    list_sum = all_values[start]
    while end < len(all_values) - 1:
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
    return 0


if __name__ == "__main__":
    main()
