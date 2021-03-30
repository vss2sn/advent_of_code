#!/usr/bin/python3

def normalizeData(line):
    return line.replace("bags", '').replace("bag", '').replace('.', '').strip()

def getNumberAndColour(line):
    data = line.split(' ')
    number = int(data[0])
    colour = (" ".join(data[1:])).strip()
    return number, colour

def n_inner_bags(contains, colour):
    if colour not in contains.keys():
        return 1
    else:
        count = 0
        for bag in contains[colour]:
            count += bag[0] * n_inner_bags(contains, bag[1])
        return count + 1

def main():
    f = open("../cpp/day_7_data.txt")
    contains = dict()
    for line in f:
        line = line.strip().replace('\n', '').replace('\r', '')
        line = normalizeData(line)
        outer_bag, bags_contained = line.split("contain")
        bags_contained = bags_contained.strip()
        outer_bag = normalizeData(outer_bag)
        if "no other" in bags_contained:
            continue
        inner_bags = [getNumberAndColour(bag) for bag in bags_contained.split(', ')]
        contains.update({outer_bag:inner_bags})
    count = n_inner_bags(contains, "shiny gold") - 1 # -1 to remove the shiny gold bag
    print(count)
    return count

if __name__ == "__main__":
    main()
