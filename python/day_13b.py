#!/usr/bin/env python3

def extendedGCD(a, b):
    if a == 0:
        return b, 0, 1
    gcd, prev_x, prev_y = extendedGCD(b % a, a)
    return gcd, prev_y - (int(b/a) * prev_x), prev_x

def ChineseRemainderTheorem(drs):
    prod = 1
    for dr in drs:
        prod *= dr[0]
    result = 0
    for div, rem in drs:
        prodbyd = int(prod / div)
        gcd, x, y = extendedGCD(prodbyd, div)
        result = (prod + result + (rem * prodbyd * x)) % prod
    return result


def main():
    f = open("../input/day_13_input")
    time = int(f.readline().strip().replace('\n', '').replace('\r', ''))
    buses = [[int(bus), int(bus) - i] for i, bus in enumerate(f.readline().strip().replace('\n', '').replace('\r', '').split(',')) if bus.isdigit()]
    result = ChineseRemainderTheorem(buses)
    print(result)
    return result

if __name__ == "__main__":
    main()
