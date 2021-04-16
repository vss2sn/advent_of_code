#!/usr/bin/env python3

import re


class Rule:
    def __init__(self, name, min_val1, max_val1, min_val2, max_val2):
        self.name = name
        self.min_val1 = int(min_val1)
        self.max_val1 = int(max_val1)
        self.min_val2 = int(min_val2)
        self.max_val2 = int(max_val2)

    def valid(self, val):
        return (val >= self.min_val1 and val <= self.max_val1) or (
            val >= self.min_val2 and val <= self.max_val2
        )


def getRules(f):
    pattern = re.compile(r"(.*?): (\d+)-(\d+) or (\d+)-(\d+)(.*?)")
    rules = dict()
    for line in f:
        line = line.strip().replace("\r", "").replace("\n", "")
        if re.match(pattern, line):
            vals = re.match(pattern, line).groups()
            rules[vals[0]] = Rule(vals[0], vals[1], vals[2], vals[3], vals[4])
        else:
            break
    return rules


def getTickets(f, rules):
    pattern = re.compile(r"(.*?):((.*?))")
    tser = 0

    for line in f:
        if re.match(pattern, line):
            continue
        line = line.strip().replace("\r", "").replace("\n", "")
        if line == "":
            continue
        else:
            vals = line.split(",")

            for val in vals:
                if not any(rule.valid(int(val)) for rule in rules.values()):
                    tser += int(val)
    return tser


def main():
    f = open("../input/day_16_input")
    rules = getRules(f)
    tser = getTickets(f, rules)
    print(tser)
    return tser


if __name__ == "__main__":
    main()
