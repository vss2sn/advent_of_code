#!/usr/bin/python3


def normalizeData(line):
    return line.replace("bags", "").replace("bag", "").replace(".", "").strip()


def getNumberAndColour(line):
    data = line.split(" ")
    number = int(data[0])
    colour = (" ".join(data[1:])).strip()
    return number, colour


def n_outer_bag(is_contained_by, colour, outer_bag_set):
    if colour in is_contained_by.keys():
        for bag in is_contained_by[colour]:
            if bag not in outer_bag_set:
                outer_bag_set.add(bag)
                n_outer_bag(is_contained_by, bag, outer_bag_set)


def main():
    f = open("../input/day_07_input")
    is_contained_by = dict()
    for line in f:
        line = line.strip().replace("\n", "").replace("\r", "")
        line = normalizeData(line)
        outer_bag, bags_contained = line.split("contain")
        bags_contained = bags_contained.strip()
        outer_bag = normalizeData(outer_bag)
        if "no other" in bags_contained:
            continue
        for bag in bags_contained.split(", "):
            bag_nc = getNumberAndColour(bag)
            if bag_nc[1] not in is_contained_by.keys():
                is_contained_by[bag_nc[1]] = list()
            is_contained_by[bag_nc[1]].append(outer_bag)

    outer_bag_set = set()
    n_outer_bag(is_contained_by, "shiny gold", outer_bag_set)
    print(len(outer_bag_set))


if __name__ == "__main__":
    main()
