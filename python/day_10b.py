#!/usr/bin/env python3

lower_delta = 1
upper_delta = 3

def NArrangements(index, jolts, n_arr):
    if jolts[index] in n_arr.keys():
        return n_arr[jolts[index]]
    count = 0
    for i in range(lower_delta, upper_delta + 1):
        if index - i >= 0 and jolts[index] - jolts[index - i] <= 3:
            count += NArrangements(index - i, jolts, n_arr)
    n_arr[jolts[index]] = count
    return count

def main():
    f = open("../cpp/day_10_data.txt")
    jolts = list()
    for line in f:
        line = line.strip().replace('\n', '').replace('\r', '')
        jolts.append(int(line))
    jolts.append(0)
    jolts.sort()
    n_arr = dict()
    n_arr[0] = 1
    count = NArrangements(len(jolts) - 1, jolts, n_arr)
    print(count)
    return count

if __name__ == "__main__":
    main()
