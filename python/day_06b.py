#!/usr/bin/env python3

def createSet():
    myset = set()
    for val in range(ord('a'), ord('z') + 1):
        myset.add(chr(val))
    # print(len(myset))
    # print(myset)
    return myset

def main():
    f = open("../input/day_6_input")
    count = 0
    answers = createSet()
    for line in f:
        line = line.strip().replace('\r','').replace('\n','')
        if line == "":
            count += len(answers)
            answers = createSet()
        else:
            answers = {letter for letter in answers if letter in line}
    count += len(answers)
    print(count)
    return count

if __name__ == "__main__":
    main()
