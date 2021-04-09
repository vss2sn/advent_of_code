#!/usr/bin/env python3


def main():
    list_size = 25
    f = open("../input/day_09_input")
    values = list()
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
        else:
            print(new_number)
            return new_number
    return 0


if __name__ == "__main__":
    main()
