#!/usr/bin/env python3

def main():
    f = open("../cpp/day_6_data.txt")
    count = 0
    answers = set()
    for line in f:
        line = line.strip().replace('\r','').replace('\n','')
        if line == "":
            count += len(answers)
            answers = set()
        else:
            for letter in line:
                answers.add(letter)
    count += len(answers)
    print(count)
    return count

if __name__ == "__main__":
    main()
