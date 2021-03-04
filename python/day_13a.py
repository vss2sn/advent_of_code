#!/usr/bin/env python3

def main():
    f = open("../cpp/day_13_data.txt")
    time = int(f.readline().strip().replace('\n', '').replace('\r', ''))
    buses = [int(bus) for bus in f.readline().strip().replace('\n', '').replace('\r', '').replace('x,', '').split(',')]
    rem  = [min(buses), buses.index(min(buses))]
    for bus in buses:
        c_rem = bus - (time % bus)
        if c_rem < rem[0]:
            rem[0] = c_rem
            rem[1] = bus
    print(rem[0] * rem[1])
    return rem[0] * rem[1]

if __name__ == "__main__":
    main()
