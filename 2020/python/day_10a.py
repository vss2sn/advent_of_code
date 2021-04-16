#!/usr/bin/env python3


def main():
    f = open("../input/day_10_input")
    jolts = list()
    for line in f:
        line = line.strip().replace("\n", "").replace("\r", "")
        jolts.append(int(line))
    jolts.sort()
    diff_1 = 0
    diff_2 = 0
    diff_3 = 0
    for i in range(1, len(jolts)):
        diff = jolts[i] - jolts[i - 1]
        if diff == 1:
            diff_1 += 1
        elif diff == 2:
            diff_2 += 1
        elif diff == 3:
            diff_3 += 1
        else:
            print("WTH")
    diff_1 += 1  # from outlet to first
    diff_3 += 1  # to built in adapter
    print(diff_1 * diff_3)
    return diff_1 * diff_3


if __name__ == "__main__":
    main()
