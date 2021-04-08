#!/usr/bin/env python3


def main():
    f = open("../input/day_4_input")
    key_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}
    eye_colours = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}
    rules = {
        "byr": lambda x: int(x) >= 1920 and int(x) <= 2002,
        "iyr": lambda x: int(x) >= 2010 and int(x) <= 2020,
        "eyr": lambda x: int(x) >= 2020 and int(x) <= 2030,
        "hgt": lambda x: (x[-2:] == "cm" and int(x[:-2]) >= 150 and int(x[:-2]) <= 193)
        or (x[-2:] == "in" and int(x[:-2]) >= 59 and int(x[:-2]) <= 76),
        "hcl": lambda x: x[0] == "#"
        and all(char in "0123456789abcdef" for char in x[1:]),
        "ecl": lambda x: x in eye_colours,
        "pid": lambda x: len(x) == 9 and x.isnumeric(),
    }
    n_valid = 0
    n_fields = 0
    for line in f:
        line = line.strip().replace("\n", "").replace("\r", "")
        if len(line) == 0:
            if n_fields == len(key_fields):
                n_valid += 1
            n_fields = 0
            continue
        fields = line.split()
        for field in fields:
            split_field = field.split(":")
            if split_field[0].strip() in key_fields and rules[split_field[0].strip()](
                split_field[1].strip()
            ):
                n_fields += 1
    if n_fields == len(key_fields):
        n_valid += 1
    print(n_valid)
    return n_valid


if __name__ == "__main__":
    main()
