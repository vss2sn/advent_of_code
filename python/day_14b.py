#!/usr/bin/env python3

import copy
import itertools
import re


def getAddresses(mask, address):
    address_list = [int(x) for x in bin(int(address))[2:]]
    address_list = [0] * (36 - len(address_list)) + address_list
    for index in range(0, 36):
        if mask[index] == "1":
            address_list[index] = 1
    addresses = list()
    n = mask.count("X")
    for perm in itertools.product([0, 1], repeat=n):
        count = 0
        for index in range(0, 36):
            if mask[index] == "X":
                address_list[index] = perm[count]
                count += 1
        res = 0
        for ele in address_list:
            res = (res << 1) | ele
        addresses.append(res)
    return addresses


def main():
    f = open("../input/day_14_input")
    mem_pattern = re.compile(r"mem\[(\d+)\] = (\d+)(.*?)")
    mem = dict()
    for line in f:
        line = line.strip().replace("\n", "").replace("\r", "")
        if line[0:4] == "mask":
            mask = line.split()[-1]
        else:
            address, value, __ = mem_pattern.match(line).groups()
            addresses = getAddresses(mask, address)
            for addr in addresses:
                mem[addr] = int(value)
    total = 0
    for ele in mem.values():
        total += ele
    print(total)
    return total


if __name__ == "__main__":
    main()
