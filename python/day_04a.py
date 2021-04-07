#!/usr/bin/env python3

def main():
    f = open("../input/day_4_input")
    key_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}
    n_valid = 0;
    n_fields = 0;
    n_lines = 0;
    for line in f:
        line = line.strip().replace('\n', '').replace('\r', '')
        if len(line) == 0:
            if(n_fields == len(key_fields)):
                n_valid += 1
            n_fields = 0
            continue
        fields = line.split()
        for field in fields:
            split_field = field.split(":")
            if split_field[0].strip() in key_fields:
                n_fields += 1
    if(n_fields == len(key_fields)):
        n_valid += 1
    print(n_valid)
    return n_valid

if __name__ == "__main__":
    main()
